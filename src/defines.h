/*
 * Title:			AGON MOS - MOS defines
 * Author:			Dean Belfield
 * Created:			21/03/2023
 * Last Updated:	10/11/2023
 * 
 * Modinfo:
 * 22/03/2023:		The VDP commands are now indexed from 0x80
 * 24/03/2023:		Added DEBUG
 * 10/11/2023:		Added VDP_consolemode
 */

#ifndef MOS_DEFINES_H
#define MOS_DEFINES_H

#define	DEBUG					0			// Set to 0 for production, 1 for extra debug information

#include <stdint.h>
#include <stdbool.h>

/* For sanity */
typedef int int24_t;
typedef unsigned int uint24_t;

/* Some legacy types to avoid huge search/replace right now */
typedef uint32_t UINT32;
typedef int32_t INT32;
typedef uint24_t UINT ;
typedef uint24_t UINT24 ;
typedef int24_t INT24 ;
typedef int24_t INT ;
typedef uint16_t UINT16 ;
typedef int16_t INT16 ;
typedef uint8_t UINT8 ;
typedef int8_t INT8 ;
typedef uint8_t BYTE;
typedef uint8_t UINT8;
typedef uint8_t UCHAR;
typedef bool BOOL;

#define TRUE true
#define FALSE false

// ZDS segment stuff
extern int8_t __heapbot[];
extern int8_t __heaptop[];
extern int8_t _stack[];
extern int8_t __rodata_end[];
extern int8_t __data_start[];
extern int8_t __data_len[];
extern int8_t _low_romdata[];
extern int _len_data;


#define SPL_STACK_SIZE				2048
#define HEAP_LEN ((int)_stack - (int)__heapbot - SPL_STACK_SIZE)

// VDP specific (for VDU 23,0,n commands)
//
#define VDP_gp			 		0x80
#define VDP_keycode				0x81
#define VDP_cursor				0x82
#define VDP_scrchar				0x83
#define VDP_scrpixel			0x84
#define VDP_audio				0x85
#define VDP_mode				0x86
#define VDP_rtc					0x87
#define VDP_keystate			0x88
#define VDP_palette             0x94
#define VDP_logicalcoords		0xC0
#define VDP_consolemode			0xFE
#define VDP_terminalmode		0xFF

#endif /* MOS_DEFINES_H */
