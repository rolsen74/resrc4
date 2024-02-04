
/*
 * Copyright (c) 2014-2024 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "ReSrc4.h"

// --

static int Global_Dos_Used = false;

struct lvostruct
{
	int16_t	Used;
	int16_t	Offset;
	char *	Name;
	void 	(*Func)( struct M68kStruct *ms );
};

// --


// --

static struct lvostruct LVOS[] =
{
	{ 0, -30, "_LVOOpen", Mark_D1_NulString },
	{ 0, -36, "_LVOClose", NULL },
	{ 0, -42, "_LVORead", NULL },
	{ 0, -48, "_LVOWrite", NULL },
	{ 0, -54, "_LVOInput", NULL },
	{ 0, -60, "_LVOOutput", NULL },
	{ 0, -66, "_LVOSeek", NULL },
	{ 0, -72, "_LVODeleteFile", Mark_D1_NulString },
	{ 0, -78, "_LVORename", Mark_D1D2_NulString },
	{ 0, -84, "_LVOLock", Mark_D1_NulString },
	{ 0, -90, "_LVOUnLock", NULL },
	{ 0, -96, "_LVODupLock", NULL },
	{ 0, -102, "_LVOExamine", NULL },
	{ 0, -108, "_LVOExNext", NULL },
	{ 0, -114, "_LVOInfo", NULL },
	{ 0, -120, "_LVOCreateDir", Mark_D1_NulString },
	{ 0, -126, "_LVOCurrentDir", NULL },
	{ 0, -132, "_LVOIoErr", NULL },
	{ 0, -138, "_LVOCreateProc", Mark_D1_NulString },
	{ 0, -144, "_LVOExit", NULL },
	{ 0, -150, "_LVOLoadSeg", Mark_D1_NulString },
	{ 0, -156, "_LVOUnLoadSeg", NULL },
	{ 0, -174, "_LVODeviceProc", Mark_D1_NulString },
	{ 0, -180, "_LVOSetComment", Mark_D1D2_NulString },
	{ 0, -186, "_LVOSetProtection", Mark_D1_NulString },
	{ 0, -192, "_LVODateStamp", NULL },
	{ 0, -198, "_LVODelay", NULL },
	{ 0, -204, "_LVOWaitForChar", NULL },
	{ 0, -210, "_LVOParentDir", NULL },
	{ 0, -216, "_LVOIsInteractive", NULL },
	{ 0, -222, "_LVOExecute", Mark_D1_NulString },
	{ 0, -228, "_LVOAllocDosObject", NULL },
	{ 0, -234, "_LVOFreeDosObject", NULL },
	{ 0, -240, "_LVODoPkt", NULL },
	{ 0, -246, "_LVOSendPkt", NULL },
	{ 0, -252, "_LVOWaitPkt", NULL },
	{ 0, -258, "_LVOReplyPkt", NULL },
	{ 0, -264, "_LVOAbortPkt", NULL },
	{ 0, -270, "_LVOLockRecord", NULL },
	{ 0, -276, "_LVOLockRecords", NULL },
	{ 0, -282, "_LVOUnLockRecord", NULL },
	{ 0, -288, "_LVOUnLockRecords", NULL },
	{ 0, -294, "_LVOSelectInput", NULL },
	{ 0, -300, "_LVOSelectOutput", NULL },
	{ 0, -306, "_LVOFGetC", NULL },
	{ 0, -312, "_LVOFPutC", NULL },
	{ 0, -318, "_LVOUnGetC", NULL },
	{ 0, -324, "_LVOFRead", NULL },
	{ 0, -330, "_LVOFWrite", NULL },
	{ 0, -336, "_LVOFGets", NULL },
	{ 0, -342, "_LVOFPuts", Mark_D2_NulString },
	{ 0, -348, "_LVOVFWritef", NULL },
	{ 0, -354, "_LVOVFPrintf", Mark_D2_NulString },
	{ 0, -360, "_LVOFlush", NULL },
	{ 0, -366, "_LVOSetVBuf", NULL },
	{ 0, -372, "_LVODupLockFromFH", NULL },
	{ 0, -378, "_LVOOpenFromLock", NULL },
	{ 0, -384, "_LVOParentOfFH", NULL },
	{ 0, -390, "_LVOExamineFH", NULL },
	{ 0, -396, "_LVOSetFileDate", Mark_D1_NulString },
	{ 0, -402, "_LVONameFromLock", NULL },
	{ 0, -408, "_LVONameFromFH", NULL },
	{ 0, -414, "_LVOSplitName", Mark_D1_NulString },
	{ 0, -420, "_LVOSameLock", NULL },
	{ 0, -426, "_LVOSetMode", NULL },
	{ 0, -432, "_LVOExAll", NULL },
	{ 0, -438, "_LVOReadLink", Mark_D3_NulString },
	{ 0, -444, "_LVOMakeLink", Mark_D1_NulString },
	{ 0, -450, "_LVOChangeMode", NULL },
	{ 0, -456, "_LVOSetFileSize", NULL },
	{ 0, -462, "_LVOSetIoErr", NULL },
	{ 0, -468, "_LVOFault", NULL },
	{ 0, -474, "_LVOPrintFault", Mark_D2_NulString },
	{ 0, -480, "_LVOErrorReport", NULL },
	{ 0, -492, "_LVOCli", NULL },
	{ 0, -498, "_LVOCreateNewProc", NULL },
	{ 0, -504, "_LVORunCommand", Mark_D3_NulString },
	{ 0, -510, "_LVOGetConsoleTask", NULL },
	{ 0, -516, "_LVOSetConsoleTask", NULL },
	{ 0, -522, "_LVOGetFileSysTask", NULL },
	{ 0, -528, "_LVOSetFileSysTask", NULL },
	{ 0, -534, "_LVOGetArgStr", NULL },
	{ 0, -540, "_LVOSetArgStr", Mark_D1_NulString },
	{ 0, -546, "_LVOFindCliProc", NULL },
	{ 0, -552, "_LVOMaxCli", NULL },
	{ 0, -558, "_LVOSetCurrentDirName", Mark_D1_NulString },
	{ 0, -564, "_LVOGetCurrentDirName", NULL },
	{ 0, -570, "_LVOSetProgramName", Mark_D1_NulString },
	{ 0, -576, "_LVOGetProgramName", NULL },
	{ 0, -582, "_LVOSetPrompt", Mark_D1_NulString },
	{ 0, -588, "_LVOGetPrompt", NULL },
	{ 0, -594, "_LVOSetProgramDir", NULL },
	{ 0, -600, "_LVOGetProgramDir", NULL },
	{ 0, -606, "_LVOSystemTagList", Mark_D1_NulString },
	{ 0, -612, "_LVOAssignLock", Mark_D1D2_NulString },
	{ 0, -618, "_LVOAssignLate", Mark_D1D2_NulString },
	{ 0, -624, "_LVOAssignPath", Mark_D1D2_NulString },
	{ 0, -630, "_LVOAssignAdd", Mark_D1_NulString },
	{ 0, -636, "_LVORemAssignList", Mark_D1_NulString },
	{ 0, -642, "_LVOGetDeviceProc", Mark_D1_NulString },
	{ 0, -648, "_LVOFreeDeviceProc", NULL },
	{ 0, -654, "_LVOLockDosList", NULL },
	{ 0, -660, "_LVOUnLockDosList", NULL },
	{ 0, -666, "_LVOAttemptLockDosList", NULL },
	{ 0, -672, "_LVORemDosEntry", NULL },
	{ 0, -678, "_LVOAddDosEntry", NULL },
	{ 0, -684, "_LVOFindDosEntry", Mark_D2_NulString },
	{ 0, -690, "_LVONextDosEntry", NULL },
	{ 0, -696, "_LVOMakeDosEntry", Mark_D1_NulString },
	{ 0, -702, "_LVOFreeDosEntry", NULL },
	{ 0, -708, "_LVOIsFileSystem", Mark_D1_NulString },
	{ 0, -714, "_LVOFormat", Mark_D1D2_NulString },
	{ 0, -720, "_LVORelabel", Mark_D1D2_NulString },
	{ 0, -726, "_LVOInhibit", Mark_D1_NulString },
	{ 0, -732, "_LVOAddBuffers", Mark_D1_NulString },
	{ 0, -738, "_LVOCompareDates", NULL },
	{ 0, -744, "_LVODateToStr", NULL },
	{ 0, -750, "_LVOStrToDate", NULL },
	{ 0, -756, "_LVOInternalLoadSeg", NULL },
	{ 0, -762, "_LVOInternalUnLoadSeg", NULL },
	{ 0, -768, "_LVONewLoadSeg", Mark_D1_NulString },
	{ 0, -774, "_LVOAddSegment", Mark_D1_NulString },
	{ 0, -780, "_LVOFindSegment", Mark_D1_NulString },
	{ 0, -786, "_LVORemSegment", NULL },
	{ 0, -792, "_LVOCheckSignal", NULL },
	{ 0, -798, "_LVOReadArgs", Mark_D1_NulString },
	{ 0, -804, "_LVOFindArg", Mark_D1D2_NulString },
	{ 0, -810, "_LVOReadItem", NULL },
	{ 0, -816, "_LVOStrToLong", Mark_D1_NulString },
	{ 0, -822, "_LVOMatchFirst", Mark_D1_NulString },
	{ 0, -828, "_LVOMatchNext", NULL },
	{ 0, -834, "_LVOMatchEnd", NULL },
	{ 0, -840, "_LVOParsePattern", Mark_D1_NulString },
	{ 0, -846, "_LVOMatchPattern", Mark_D2_NulString },
	{ 0, -858, "_LVOFreeArgs", NULL },
	{ 0, -870, "_LVOFilePart", Mark_D1_NulString },
	{ 0, -876, "_LVOPathPart", Mark_D1_NulString },
	{ 0, -882, "_LVOAddPart", Mark_D1D2_NulString },
	{ 0, -888, "_LVOStartNotify", NULL },
	{ 0, -894, "_LVOEndNotify", NULL },
	{ 0, -900, "_LVOSetVar", Mark_D1_NulString },
	{ 0, -906, "_LVOGetVar", Mark_D1_NulString },
	{ 0, -912, "_LVODeleteVar", Mark_D1_NulString },
	{ 0, -918, "_LVOFindVar", Mark_D1_NulString },
	{ 0, -930, "_LVOCliInitNewcli", NULL },
	{ 0, -936, "_LVOCliInitRun", NULL },
	{ 0, -942, "_LVOWriteChars", NULL },
	{ 0, -948, "_LVOPutStr", Mark_D1_NulString },
	{ 0, -954, "_LVOVPrintf", Mark_D1_NulString },
	{ 0, -966, "_LVOParsePatternNoCase", Mark_D1_NulString },
	{ 0, -972, "_LVOMatchPatternNoCase", Mark_D1_NulString },
	{ 0, -984, "_LVOSameDevice", NULL },
	{ 0, -990, "_LVOExAllEnd", NULL },
	{ 0, -996, "_LVOSetOwner", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Dos( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Dos_Used = true;
			LVOS[pos].Used = true;
			lvo = LVOS[pos].Name;

			if ( LVOS[pos].Func )
			{
				LVOS[pos].Func( ms );
			}
			break;
		}
		else
		{
			pos++;
		}
	}

	if ( lvo == NULL )
	{
printf( "Unknown Dos libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Dos\n";

int Save_Lib_Dos( void )
{
int error;
int pos;
int len;

	if ( Global_Dos_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Dos lvo's\n", __FILE__, __LINE__ );
		goto bailout;
	}

	pos = 0;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Used )
		{
			len = strlen( LVOS[pos].Name ) + 1;

			if ( len < 8 )
			{
				sprintf( SaveLineBuffer, "%s:\t\t\tEQU\t%d\n", LVOS[pos].Name, LVOS[pos].Offset );
			}
			else if ( len < 16 )
			{
				sprintf( SaveLineBuffer, "%s:\t\tEQU\t%d\n", LVOS[pos].Name, LVOS[pos].Offset );
			}
			else if ( len < 24 )
			{
				sprintf( SaveLineBuffer, "%s:\tEQU\t%d\n", LVOS[pos].Name, LVOS[pos].Offset );
			}
			else
			{
				sprintf( SaveLineBuffer, "%s: EQU\t%d\n", LVOS[pos].Name, LVOS[pos].Offset );
			}

			error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

			if ( error )
			{
				printf( "%s:%04d: Error Writting Dos Data\n", __FILE__, __LINE__ );
				goto bailout;
			}
		}

		pos++;
	}

	error = false;

bailout:

	return( error );
}

// --
