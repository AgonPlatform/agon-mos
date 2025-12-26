/*
 * Title:			AGON MOS
 * Author:			Dean Belfield
 * Created:			19/06/2022
 * Last Updated:	11/11/2023
 *
 * Modinfo:
 * 11/07/2022:		Version 0.01: Tweaks for Agon Light, Command Line code added
 * 13/07/2022:		Version 0.02
 * 15/07/2022:		Version 0.03: Warm boot support, VBLANK interrupt
 * 25/07/2022:		Version 0.04; Tweaks to initialisation and interrupts
 * 03/08/2022:		Version 0.05: Extended MOS for BBC Basic, added config file
 * 05/08/2022:		Version 0.06: Interim release with hardware flow control enabled
 * 10/08/2022:		Version 0.07: Bug fixes
 * 05/09/2022:		Version 0.08: Minor updates to MOS
 * 02/10/2022:		Version 1.00: Improved error handling for languages, changed bootup title to Quark
 * 03/10/2022:		Version 1.01: Added SET command, tweaked error handling
 * 20/10/2022:					+ Tweaked error handling
 * 13/11/2022:		Version 1.02
 * 14/03/2023		Version 1.03: SD now uses timer0, does not require interrupt
 *								+ Stubbed command history
 * 22/03/2023:					+ Moved command history to mos_editor.c
 * 23/03/2023:				RC2	+ Increased baud rate to 1152000
 * 								+ Improved ESP32->eZ80 boot sync
 * 29/03/2023:				RC3 + Added UART1 initialisation, tweaked startup sequence timings
 * 16/05/2023:		Version 1.04: Fixed MASTERCLOCK value in uart.h, added startup beep
 * 03/08/2023:				RC2	+ Enhanced low-level keyboard functionality
 * 27/09/2023:					+ Updated RTC
 * 11/11/2023:				RC3	+ See Github for full list of changes
 */

#include "ez80f92.h"
#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "version.h"
#include "config.h"
#include "uart.h"
#include "spi.h"
#include "timer.h"
#include "ff.h"
#include "clock.h"
#include "mos_editor.h"
#include "mos.h"
#include "i2c.h"
#include "umm_malloc.h"

extern volatile BYTE scrcolours, scrpixelIndex;  // In globals.asm

extern void *	set_vector(unsigned int vector, void(*handler)(void));

extern void 	vblank_handler(void);
extern void 	uart0_handler(void);
extern void 	i2c_handler(void);

extern char hardReset;		// 1 = hard cpu reset, 0 = soft reset
extern volatile	char 	keycode;		// Keycode 
extern volatile char	gp;				// General poll variable

extern BOOL	vdpSupportsTextPalette;

// Wait for the ESP32 to respond with a GP packet to signify it is ready
// Parameters:
// - pUART: Pointer to a UART structure
// - baudRate: Baud rate to initialise UART with
// Returns:
// - 1 if the function succeeded, otherwise 0
//
int wait_ESP32(UINT24 baudRate) {	
	UART 	UART0;
	int	i, t;

	UART0.baudRate = baudRate;			// Initialise the UART object
	UART0.dataBits = 8;
	UART0.stopBits = 1;
	UART0.parity = PAR_NOPARITY;
	UART0.flowControl = FCTL_HW;
	UART0.interrupts = UART_IER_RECEIVEINT;

	open_UART0(&UART0);					// Open the UART 
	init_timer0(10, 16, 0x00);  		// 10ms timer for delay
	gp = 0;								// Reset the general poll byte	
	for(t = 0; t < 200; t++) {			// A timeout loop (200 x 50ms = 10s)
		putch(23);						// Send a general poll packet
		putch(0);
		putch(VDP_gp);
		putch(1);
		for(i = 0; i < 5; i++) {		// Wait 50ms
			wait_timer0();
		}
		if(gp == 1) break;				// If general poll returned, then exit for loop
	}
	enable_timer0(0);					// Disable the timer
	return gp;
}

// Initialise the interrupts
//
static void init_interrupts(void) {
	set_vector(PORTB1_IVECT, vblank_handler); 	// 0x32
	set_vector(UART0_IVECT, uart0_handler);		// 0x18
	set_vector(I2C_IVECT, i2c_handler);			// 0x1C
}

static uint8_t quickrand(void) {
	uint8_t out;
	asm volatile("ld a,r\n":"=%a"(out));
	return out;
}

void rainbow_msg(char* msg) {
	BYTE i = quickrand() & (scrcolours - 1);
	if (strcmp(msg, "Rainbow") != 0) {
		printf("%s", msg);
		return;
	}
	if (i == 0)
		i++;
	for (; *msg; msg++) {
		putch(17);
		putch(i);
		putch(*msg);
		i = (i + 1 < scrcolours) ? i + 1 : 1;
	}
	putch(17);
	putch(15);
}

void bootmsg(void) {
	printf("Agon ");
	rainbow_msg(VERSION_VARIANT);
	printf(" MOS Version %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	#if VERSION_CANDIDATE > 0
		printf(" %s%d", VERSION_TYPE, VERSION_CANDIDATE);
	#endif
	// Show version subtitle, if we have one
	#ifdef VERSION_SUBTITLE
		printf(" ");
		rainbow_msg(VERSION_SUBTITLE);
	#endif
	// Show build if defined (intended to be auto-generated string from build script from git commit hash)
	#ifdef VERSION_BUILD
		printf(" Build %s", VERSION_BUILD);
	#endif

	printf("\n\r\n\r");
}


// The main loop
//
int main(void) {
	asm volatile("di");
	init_interrupts();								// Initialise the interrupt vectors
	init_rtc();										// Initialise the real time clock
	init_spi();										// Initialise SPI comms for the SD card interface
	init_UART0();									// Initialise UART0 for the ESP32 interface
	init_UART1();									// Initialise UART1
	asm volatile("ei");
	
	if(!wait_ESP32(1152000)) {				// Try to lock onto the ESP32 at maximum rate
		if(!wait_ESP32(384000))	{		// If that fails, then fallback to the lower baud rate
			gp = 2;									// Flag GP as 2, just in case we need to handle this error later
		}
	}	
	if(hardReset == 0) {
		// clear screen on soft reset, since VDP has not been reset
		putch(12);
	}

	umm_init_heap((void*)__heapbot, HEAP_LEN);

	scrcolours = 0;
	scrpixelIndex = 255;
	getModeInformation();
    while (scrcolours == 0) { }
	readPalette(128, TRUE);

	if (scrpixelIndex < 128) {
		vdpSupportsTextPalette = TRUE;
	} else {
		// VDP doesn't properly support text colour reading
		// so we may have printed a duff character to screen
		// home cursor and go down a row
		putch(0x1E);
		putch(0x0A);
	}

	bootmsg();

	mos_mount();									// Mount the SD card

	putch(7);										// Startup beep
	editHistoryInit();								// Initialise the command history

	// Load the autoexec.bat config file
	//
	#if enable_config == 1
	{
		int err = mos_EXEC("autoexec.txt", cmd, sizeof cmd);	// Then load and run the config file
		if (err > 0 && err != FR_NO_FILE) {
			mos_error(err);
		}
	}
	#endif

	// The main loop
	//
	while(1) {
		if(mos_input(cmd, sizeof(cmd)) == 13) {
			int err = mos_exec(cmd, TRUE);
			if(err > 0) {
				mos_error(err);
			}
		}
		else {
			printf("Escape\n\r");
		}
	}

	return 0;
}
