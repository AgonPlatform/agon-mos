/*
 * Title:			AGON MOS - MOS code
 * Author:			Dean Belfield
 * Created:			10/07/2022
 * Last Updated:	11/11/2023
 *
 * Modinfo:
 * 11/07/2022:		Removed mos_cmdBYE, Added mos_cmdLOAD
 * 12/07/2022:		Added mos_cmdJMP
 * 13/07/2022:		Added mos_cmdSAVE
 * 14/07/2022:		Added mos_cmdRUN
 * 24/07/2022:		Added mos_getkey
 * 03/08/2022:		Added a handful of MOS API calls
 * 05/08/2022:		Added mos_FEOF
 * 05/09/2022:		Added mos_cmdREN, mos_cmdBOOT; moved mos_EDITLINE into mos_editline.c
 * 25/09/2022:		Added mos_GETERROR, mos_MKDIR
 * 13/10/2022:		Added mos_OSCLI and supporting code
 * 20/10/2022:		Tweaked error handling
 * 13/11/2022:		Added mos_cmp
 * 21/11/2022:		Added support for passing params to executables & ADL mode
 * 14/02/2023:		Added mos_cmdVDU
 * 20/02/2023:		Function mos_getkey now returns a BYTE
 * 09/03/2023:		Added mos_cmdTIME, mos_cmdCREDITS, mos_DIR now accepts a path
 * 14/03/2023:		Added mos_cmdCOPY and mos_COPY
 * 15/03/2023:		Added mos_GETRTC, mos_SETRTC
 * 21/03/2023:		Added mos_SETINTVECTOR
 * 14/04/2023:		Added fat_EOF
 * 15/04/2023:		Added mos_GETFIL, mos_FREAD, mos_FWRITE, mos_FLSEEK
 * 30/05/2023:		Function mos_FGETC now returns EOF flag
 * 08/07/2023		Added mos_trim function
 * 11/11/2023:		Added mos_cmdHELP, mos_cmdTYPE, mos_cmdCLS, mos_cmdMOUNT
 */

#ifndef MOS_H
#define MOS_H

#include "ff.h"
#include "defines.h"
#include "mos_sysvars.h"

extern char  	cmd[256];				// Array for the command line handler

typedef struct {
	char * name;
	int (*func)(char * ptr);
	bool expandArgs;
	char * args;
	char * help;
} t_mosCommand;

typedef struct {
	UINT8	free;
	FIL		fileObject;
} t_mosFileObject;

void 	mos_error(int error);

BYTE	mos_getkey(void);
UINT24	mos_input(char * buffer, int bufferLength);
t_mosCommand	*mos_getCommand(char * ptr, uint8_t flags);
char *	mos_trim(char * s, bool removeLeadingAsterisks, bool removeTrailingSpaces);
int		mos_runBin(UINT24 addr, char * args);
int		mos_runBinFile(char * filepath, char * args);
int		mos_runOrLoadFile(char * args, bool run);
int		mos_execAlias(char * token, char * args, char * saveToken, BOOL in_mos);
int		mos_exec(char * buffer, BOOL in_mos);
UINT8 	mos_execMode(UINT8 * ptr);

int		mos_mount(void);

void	mos_setupSystemVariables();

void	printEscapedString(char * value);
void	autoPageOutput();

int		mos_cmdCD(char * ptr);
int		mos_cmdCLS(char *ptr);
int		mos_cmdCOPY(char *ptr);
int		mos_cmdCREDITS(char *ptr);
int		mos_cmdDEL(char * ptr);
int		mos_cmdDIR(char * ptr);
int		mos_cmdDISC(char *ptr);
int		mos_cmdDO(char *ptr);
int		mos_cmdECHO(char *ptr);
int		mos_cmdEXEC(char * ptr);
int		mos_cmdHELP(char *ptr);
int		mos_cmdHOTKEY(char *ptr);
int		mos_cmdIF(char *ptr);
int		mos_cmdIFTHERE(char *ptr);
int		mos_cmdJMP(char * ptr);
int		mos_cmdLOAD(char * ptr);
int		mos_cmdLOADFILE(char * ptr);
int		mos_cmdMEM(char *ptr);
int		mos_cmdMKDIR(char *ptr);
int		mos_cmdMOUNT(char *ptr);
int		mos_cmdOBEY(char *ptr);
int		mos_cmdPRINTF(char *ptr);
int		mos_cmdREN(char *ptr);
int		mos_cmdRUN(char * ptr);
int		mos_cmdRUNBIN(char * ptr);
int		mos_cmdRUNFILE(char * ptr);
int		mos_cmdSAVE(char *ptr);
int		mos_cmdSET(char *ptr);
int		mos_cmdSETEVAL(char *ptr);
int		mos_cmdSETMACRO(char *ptr);
int		mos_cmdSHOW(char *ptr);
int		mos_cmdTIME(char *ptr);
int		mos_cmdTRY(char *ptr);
int		mos_cmdTYPE(char *ptr);
int		mos_cmdUNSET(char *ptr);
int		mos_cmdVDU(char *ptr);
#if DEBUG > 0
int mos_cmdTEST(char *ptr);
#endif /* DEBUG */

UINT24	mos_LOAD_API(char * filename, UINT24 address, UINT24 size);
UINT24	mos_LOAD(char * filename, UINT24 address, UINT24 size);
UINT24	mos_SAVE_API(char * filename, UINT24 address, UINT24 size);
UINT24	mos_SAVE(char * filename, UINT24 address, UINT24 size);
UINT24	mos_TYPE(char * filename);
UINT24	mos_CD_API(char * path);
UINT24	mos_CD(char * path);
UINT24	mos_DIR_API(char * path);
UINT24	mos_DIR(char * path, BYTE flags);
UINT24	mos_DEL(char * filename);
UINT24	mos_REN_API(char *srcPath, char *dstPath);
UINT24	mos_REN(char *srcPath, char *dstPath, BOOL verbose);
UINT24	mos_COPY_API(char *srcPath, char *dstPath);
UINT24	mos_COPY(char *srcPath, char *dstPath, BOOL verbose);
UINT24	mos_MKDIR_API(char * filename);
UINT24	mos_MKDIR(char * filename);
UINT24 	mos_EXEC(char * filename);

UINT24	mos_FOPEN(char * filename, UINT8 mode);
UINT24	mos_FCLOSE(UINT8 fh);
UINT24	mos_FGETC(UINT8 fh);
void	mos_FPUTC(UINT8 fh, char c);
UINT24	mos_FREAD(UINT8 fh, UINT24 buffer, UINT24 btr);
UINT24	mos_FWRITE(UINT8 fh, UINT24 buffer, UINT24 btw);
UINT8  	mos_FLSEEK(UINT8 fh, UINT32 offset);
UINT8	mos_FLSEEKP(UINT8 fh, DWORD * offset);
UINT8	mos_FEOF(UINT8 fh);

void 	mos_GETERROR(UINT8 errno, UINT24 address, UINT24 size);
UINT24 	mos_OSCLI(char * cmd);
UINT8 	mos_GETRTC(UINT24 address);
void	mos_UNPACKRTC(UINT24 address, UINT8 flags);
void	mos_SETRTC(UINT24 address);
UINT24	mos_SETINTVECTOR(UINT8 vector, UINT24 address);
UINT24	mos_GETFIL(UINT8 fh);

extern TCHAR	cwd[256];
extern BOOL	sdcardDelay;

UINT8	fat_tell(FIL * fp, DWORD * offset);
UINT8	fat_EOF(FIL * fp);
UINT8	fat_size(FIL * fp, DWORD * size);
UINT8	fat_error(FIL * fp);
UINT8	fat_lseek(FIL * fp, DWORD * offset);
UINT8	fat_getfree(const TCHAR * path, DWORD * clusters, DWORD * clusterSize);

UINT8	wait_VDP(UINT8 mask);


#define MOS_DIR_LONG_LISTING		1
#define MOS_DIR_SHOW_HIDDEN			2
#define MOS_DIR_SHOW_SYSTEM			4
#define MOS_DIR_HIDE_VOLUME_INFO	8


#define HELP_CAT			"Directory listing of the current directory\r\n" \
							"The path can be a pattern to restrict which files are listed\r\n" \
							"Flags to control the display:\r\n" \
							"  -l: Long listing format\r\n" \
							"  -a: Show hidden files\r\n" \
							"  -s: Show system files\r\n" \
							"  -v: Hide volume information\r\n"
#define HELP_CAT_ARGS		"[-l] [-a] [-s] [-v] <path>"

#define HELP_CD				"Change current directory\r\n"
#define HELP_CD_ARGS		"<path>"

#define HELP_CLS			"Clear the screen\r\n"

#define HELP_COPY			"Create a copy of a file\r\n"
#define HELP_COPY_ARGS		"<filename1> <filename2>"

#define HELP_CREDITS		"Output credits and version numbers for\r\n" \
							"third-party libraries used in the Agon firmware\r\n"

#define HELP_DELETE			"Delete a file or folder (must be empty)\r\n"
#define HELP_DELETE_ARGS	"[-f] <filename>"

#define HELP_DO				"Run the given command, after transformation\r\n"
#define HELP_DO_ARGS		"<command>"

#define HELP_ECHO			"Echo sends a string to the VDU, after transformation\r\n"
#define HELP_ECHO_ARGS		"<string>"

#define HELP_EXEC			"Run a batch file containing MOS commands\r\n"
#define HELP_EXEC_ARGS		"<filename>"

#define HELP_HELP			"Display help on commands.\r\n"
#define HELP_HELP_ARGS		"[ <command> | all ]"

#define HELP_HOTKEY			"Store a command in one of 12 hotkey slots assigned to F1-F12\r\n\r\n" \
							"Hotkey command strings can optionally include placeholder markers to indicate\r\n" \
							"how the command should be built from the current command line.\r\n" \
							"A placeholder of \"%s\" will insert the whole of the current line,\r\n" \
							"or a numeric placeholder of the form \"%n\" will insert the argument at position n,\r\n" \
							"and a placeholder of the form \"%*n\" will insert all arguments from n to the end.\r\n" \
							"Using the Hotkey command without any arguments will list the\r\n" \
							"currently assigned command strings.\r\n"
#define HELP_HOTKEY_ARGS	"<key number> <command string>"

#define HELP_IF				"Conditionally executes another command depending on the value of an expression\r\n"
#define HELP_IF_ARGS		"<expression> Then <command> [Else <command>]"

#define HELP_IFTHERE		"Conditionally executes another command depending on the presence of a file or directory\r\n"
#define HELP_IFTHERE_ARGS	"<filename> Then <command> [Else <command>]"

#define HELP_JMP			"Jump to the specified address in memory\r\n"
#define HELP_JMP_ARGS		"<addr>"

#define HELP_LOAD			"Load a file from the SD card to the specified address.\r\n" \
							"If no `addr` parameter is passed it will " \
							"default to &40000\r\n"
#define HELP_LOAD_ARGS		"<filename> [<addr>]"

#define HELP_LOADFILE		"Load a file from disc, according to its LoadType\r\n" \
							"MOS will look up the \"LoadType\" for the file based on its extension\r\n" \
							"in variable named Alias$@LoadType_<extension>\r\n" \
							"and execute the defined command to load the file\r\n"
#define HELP_LOADFILE_ARGS	"<filename> [<args>]"

#define HELP_MEM			"Output memory statistics\r\n"

#define HELP_MKDIR			"Create a new folder on the SD card\r\n"
#define HELP_MKDIR_ARGS		"<filename>"

#define HELP_MOUNT			"(Re-)mount the MicroSD card\r\n"

#define HELP_OBEY			"Run an Obey command file\r\n" \
							"The system variable Obey$Dir is set to the directory of the obey file\r\n" \
							"and argument substitution is supported using %0 to %9,\r\n" \
							"%s to substitute in all arguments, or %*<n> for arguments from <n> to end\r\n" \
							"If the -v flag is used, each line will be echo'd before execution\r\n"
#define HELP_OBEY_ARGS		"[-v] <filename> [<arguments>]"

#define HELP_PRINTF			"Print a string to the VDU, with common unix-style escapes\r\n"
#define HELP_PRINTF_ARGS	"<string>"

#define HELP_RENAME			"Rename a file in the same folder\r\n"
#define HELP_RENAME_ARGS	"<filename1> <filename2>"

#define HELP_RUN			"Call an executable binary loaded in memory.\r\n" \
							"If no parameters are passed, then addr will " \
							"default to &40000.\r\n"
#define HELP_RUN_ARGS		"[<addr>]"

#define HELP_RUNBIN			"Run a binary file from disc\r\n" \
							"Optionally, pass parameters to the binary\r\n" \
							"MOS will automatically load and run the binary at an appropriate address\r\n" \
							"detecting Moslets based on whether it is located in the Moslet$Path\r\n" \
							"To run a binary at a specific address, use the Load and Run commands\r\n"
#define HELP_RUNBIN_ARGS	"<filename> [<args>]"

#define HELP_RUNFILE		"Run a file from disc, according to its RunType\r\n" \
							"MOS will look up the \"RunType\" for the file based on its extension\r\n" \
							"in variable named Alias$@RunType_<extension>\r\n" \
							"and execute the defined command to run the file\r\n"
#define HELP_RUNFILE_ARGS	"<filename> [<args>]"

#define HELP_SAVE			"Save a block of memory to the SD card\r\n"
#define HELP_SAVE_ARGS		"<filename> <addr> <size>"

#define HELP_SET			"Set assigns a string value to a system variable, or system option (via special variables)\r\n\r\n" \
							"Keyboard Layout\r\n" \
							"SET KEYBOARD n: Set the keyboard layout\r\n" \
							"    0: UK (default)\r\n" \
							"    1: US\r\n" \
							"    2: German\r\n" \
							"    3: Italian\r\n" \
							"    4: Spanish\r\n" \
							"    5: French\r\n" \
							"    6: Belgian\r\n" \
							"    7: Norwegian\r\n" \
							"    8: Japanese\r\n" \
							"    9: US International\r\n" \
							"   10: US International (alternative)\r\n" \
							"   11: Swiss (German)\r\n" \
							"   12: Swiss (French)\r\n" \
							"   13: Danish\r\n" \
							"   14: Swedish\r\n" \
							"   15: Portuguese\r\n" \
							"   16: Brazilian Portugese\r\n" \
							"   17: Dvorak\r\n" \
							"\r\n" \
							"Serial Console\r\n" \
							"SET CONSOLE n: Serial console\r\n" \
							"    0: Console off (default)\r\n" \
							"    1: Console on\r\n"
#define HELP_SET_ARGS		"<varname> <value>"

#define HELP_SETEVAL		"SetEval evaluates an expression and assignes it to a system variable\r\n"
#define HELP_SETEVAL_ARGS	"<varname> <expression>"

#define HELP_SETMACRO		"SetMacro assigns a macro value to a system variable, which will be expanded each time it is used\r\n"
#define HELP_SETMACRO_ARGS	"<varname> <value>"

#define HELP_SHOW			"Show lists system variables matching the name given, or all system variables if no name is specified\r\n"
#define HELP_SHOW_ARGS		"[<variablespec>]"

#define HELP_TIME			"Set and read the VDP's real-time clock\r\n"
#define HELP_TIME_ARGS		"[ <yyyy> <mm> <dd> <hh> <mm> <ss> ]"

#define HELP_TRY			"Try executes a command and catches any errors that may occur\r\n" \
							"The return code for the command is stored in Try$ReturnCode\r\n" \
							"If an error occurred then Try$Error will be updated with the error string\r\n"
#define HELP_TRY_ARGS		"<command>"

#define HELP_TYPE			"Display the contents of a file on the screen\r\n"
#define HELP_TYPE_ARGS		"<filename>"

#define HELP_UNSET			"Unset deletes a system variable\r\n"
#define HELP_UNSET_ARGS		"<varname>"

#define HELP_VDU			"Write a stream of characters to the VDP\r\n" \
							"Character values are converted to bytes before sending\r\n"
#define HELP_VDU_ARGS		"<char1> <char2> ... <charN>"

#endif MOS_H
