
/*
 * Copyright (c) 2014-2025 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "Resourcer/ReSrc4.h"

// --

// Default D1 = Label String
static AOS3_RegStruct _String_D1[] =
{
	AOS3_REG_String( M68KREGT_D1 ),
	AOS3_REG_EndMarker()
};

// Default D2 = Label String
static AOS3_RegStruct _String_D2[] =
{
	AOS3_REG_String( M68KREGT_D2 ),
	AOS3_REG_EndMarker()
};

// Default D3 = Label String
static AOS3_RegStruct _String_D3[] =
{
	AOS3_REG_String( M68KREGT_D3 ),
	AOS3_REG_EndMarker()
};

// Default D1 + D2 = Label String
static AOS3_RegStruct _String_D1D2[] =
{
	AOS3_REG_String( M68KREGT_D1 ),
	AOS3_REG_String( M68KREGT_D2 ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct AOS3_ReadLink_Regs[] =
{
//	AOS3_REG_Struct( M68KREGT_D3, RS4StructID_Interrupt ),
//	AOS3_REG_Struct( M68KREGT_D4, RS4StructID_Interrupt ),
	AOS3_REG_String( M68KREGT_D3 ),
	AOS3_REG_String( M68KREGT_D4 ),
	AOS3_REG_EndMarker()
};


#define AOS3_AddBuffers_Regs			_String_D1
#define AOS3_AddSegment_Regs			_String_D1
#define AOS3_DeleteFile_Regs			_String_D1
#define AOS3_Lock_Regs					_String_D1
#define AOS3_Open_Regs					_String_D1
#define AOS3_CreateDir_Regs				_String_D1
#define AOS3_LoadSeg_Regs				_String_D1
#define AOS3_NewLoadSeg_Regs			_String_D1
#define AOS3_Execute_Regs				_String_D1
#define AOS3_MakeLink_Regs				_String_D1
#define AOS3_VPrintf_Regs				_String_D1
#define AOS3_PutStr_Regs				_String_D1
#define AOS3_SplitName_Regs				_String_D1
#define AOS3_SetFileDate_Regs			_String_D1

#define AOS3_FPuts_Regs					_String_D2
#define AOS3_VFPrintf_Regs				_String_D2

#define AOS3_RunCommand_Regs			_String_D3

#define AOS3_Relabel_Regs				_String_D1D2
#define AOS3_Rename_Regs				_String_D1D2
#define AOS3_SetComment_Regs			_String_D1D2
#define AOS3_MatchPatternNoCase_Regs	_String_D1D2
#define AOS3_ParsePatternNoCase_Regs	_String_D1D2

// --

AOS3_LVOStruct AOS3_DosBase[] =
{
	AOS3_LVO_Regs( -30, Open ),
	AOS3_LVO_Name( -36, Close ),
	AOS3_LVO_Name( -42, Read ),
	AOS3_LVO_Name( -48, Write ),
	AOS3_LVO_Name( -54, Input ),
	AOS3_LVO_Name( -60, Output ),
	AOS3_LVO_Name( -66, Seek ),
	AOS3_LVO_Regs( -72, DeleteFile ),
	AOS3_LVO_Regs( -78, Rename ),
	AOS3_LVO_Regs( -84, Lock ),
	AOS3_LVO_Name( -90, UnLock ),
	AOS3_LVO_Name( -96, DupLock ),
	AOS3_LVO_Name( -102, Examine ),
	AOS3_LVO_Name( -108, ExNext ),
	AOS3_LVO_Name( -114, Info ),
	AOS3_LVO_Regs( -120, CreateDir ),
	AOS3_LVO_Name( -126, CurrentDir ),
	AOS3_LVO_Name( -132, IoErr ),
	AOS3_LVO_Name( -138, CreateProc ), // AOS3_Dos_Func_CreateProc },
	AOS3_LVO_Name( -144, Exit ),
	AOS3_LVO_Regs( -150, LoadSeg ),
	AOS3_LVO_Name( -156, UnLoadSeg ),
	AOS3_LVO_Name( -174, DeviceProc ), // AOS3_Dos_Func_DeviceProc },
	AOS3_LVO_Regs( -180, SetComment ),
	AOS3_LVO_Name( -186, SetProtection ), // AOS3_Dos_Func_SetProtection },
	AOS3_LVO_Name( -192, DateStamp ),
	AOS3_LVO_Name( -198, Delay ),
	AOS3_LVO_Name( -204, WaitForChar ),
	AOS3_LVO_Name( -210, ParentDir ),
	AOS3_LVO_Name( -216, IsInteractive ),
	AOS3_LVO_Regs( -222, Execute ),
	AOS3_LVO_Name( -228, AllocDosObject ),
	AOS3_LVO_Name( -234, FreeDosObject ),
	AOS3_LVO_Name( -240, DoPkt ),
	AOS3_LVO_Name( -246, SendPkt ),
	AOS3_LVO_Name( -252, WaitPkt ),
	AOS3_LVO_Name( -258, ReplyPkt ),
	AOS3_LVO_Name( -264, AbortPkt ),
	AOS3_LVO_Name( -270, LockRecord ),
	AOS3_LVO_Name( -276, LockRecords ),
	AOS3_LVO_Name( -282, UnLockRecord ),
	AOS3_LVO_Name( -288, UnLockRecords ),
	AOS3_LVO_Name( -294, SelectInput ),
	AOS3_LVO_Name( -300, SelectOutput ),
	AOS3_LVO_Name( -306, FGetC ),
	AOS3_LVO_Name( -312, FPutC ),
	AOS3_LVO_Name( -318, UnGetC ),
	AOS3_LVO_Name( -324, FRead ),
	AOS3_LVO_Name( -330, FWrite ),
	AOS3_LVO_Name( -336, FGets ),
	AOS3_LVO_Regs( -342, FPuts ),
	AOS3_LVO_Name( -348, VFWritef ),
	AOS3_LVO_Regs( -354, VFPrintf ),
	AOS3_LVO_Name( -360, Flush ),
	AOS3_LVO_Name( -366, SetVBuf ),
	AOS3_LVO_Name( -372, DupLockFromFH ),
	AOS3_LVO_Name( -378, OpenFromLock ),
	AOS3_LVO_Name( -384, ParentOfFH ),
	AOS3_LVO_Name( -390, ExamineFH ),
	AOS3_LVO_Regs( -396, SetFileDate ),
	AOS3_LVO_Name( -402, NameFromLock ),
	AOS3_LVO_Name( -408, NameFromFH ),
	AOS3_LVO_Regs( -414, SplitName ),
	AOS3_LVO_Name( -420, SameLock ),
	AOS3_LVO_Name( -426, SetMode ),
	AOS3_LVO_Name( -432, ExAll ),
	AOS3_LVO_Regs( -438, ReadLink ),
	AOS3_LVO_Regs( -444, MakeLink ),
	AOS3_LVO_Name( -450, ChangeMode ),
	AOS3_LVO_Name( -456, SetFileSize ),
	AOS3_LVO_Name( -462, SetIoErr ),
	AOS3_LVO_Name( -468, Fault ),
	AOS3_LVO_Name( -474, PrintFault ), // AOS3_Dos_Func_PrintFault },
	AOS3_LVO_Name( -480, ErrorReport ),
	AOS3_LVO_Name( -492, Cli ),
	AOS3_LVO_Name( -498, CreateNewProc ),
	AOS3_LVO_Regs( -504, RunCommand ),
	AOS3_LVO_Name( -510, GetConsoleTask ),
	AOS3_LVO_Name( -516, SetConsoleTask ),
	AOS3_LVO_Name( -522, GetFileSysTask ),
	AOS3_LVO_Name( -528, SetFileSysTask ),
	AOS3_LVO_Name( -534, GetArgStr ),
	AOS3_LVO_Name( -540, SetArgStr ), // AOS3_Dos_Func_SetArgStr },
	AOS3_LVO_Name( -546, FindCliProc ),
	AOS3_LVO_Name( -552, MaxCli ),
	AOS3_LVO_Name( -558, SetCurrentDirName ), // AOS3_Dos_Func_SetCurrentDirName },
	AOS3_LVO_Name( -564, GetCurrentDirName ),
	AOS3_LVO_Name( -570, SetProgramName ), // AOS3_Dos_Func_SetProgramName },
	AOS3_LVO_Name( -576, GetProgramName ),
	AOS3_LVO_Name( -582, SetPrompt ), // AOS3_Dos_Func_SetPrompt },
	AOS3_LVO_Name( -588, GetPrompt ),
	AOS3_LVO_Name( -594, SetProgramDir ),
	AOS3_LVO_Name( -600, GetProgramDir ),
	AOS3_LVO_Name( -606, SystemTagList ), // AOS3_Dos_Func_SystemTagList },
	AOS3_LVO_Name( -612, AssignLock ), // AOS3_Dos_Func_AssignLock },
	AOS3_LVO_Name( -618, AssignLate ), // AOS3_Dos_Func_AssignLate },
	AOS3_LVO_Name( -624, AssignPath ), // AOS3_Dos_Func_AssignPath },
	AOS3_LVO_Name( -630, AssignAdd ), // AOS3_Dos_Func_AssignAdd },
	AOS3_LVO_Name( -636, RemAssignList ), // AOS3_Dos_Func_RemAssignList },
	AOS3_LVO_Name( -642, GetDeviceProc ), // AOS3_Dos_Func_GetDeviceProc },
	AOS3_LVO_Name( -648, FreeDeviceProc ),
	AOS3_LVO_Name( -654, LockDosList ),
	AOS3_LVO_Name( -660, UnLockDosList ),
	AOS3_LVO_Name( -666, AttemptLockDosList ),
	AOS3_LVO_Name( -672, RemDosEntry ),
	AOS3_LVO_Name( -678, AddDosEntry ),
	AOS3_LVO_Name( -684, FindDosEntry ), // AOS3_Dos_Func_FindDosEntry },
	AOS3_LVO_Name( -690, NextDosEntry ),
	AOS3_LVO_Name( -696, MakeDosEntry ), // AOS3_Dos_Func_MakeDosEntry },
	AOS3_LVO_Name( -702, FreeDosEntry ),
	AOS3_LVO_Name( -708, IsFileSystem ), // AOS3_Dos_Func_IsFileSystem },
	AOS3_LVO_Name( -714, Format ), // AOS3_Dos_Func_Format },
	AOS3_LVO_Name( -720, Relabel ), // AOS3_Dos_Func_Relabel },
	AOS3_LVO_Name( -726, Inhibit ), // AOS3_Dos_Func_Inhibit },
	AOS3_LVO_Regs( -732, AddBuffers ),
	AOS3_LVO_Name( -738, CompareDates ),
	AOS3_LVO_Name( -744, DateToStr ),
	AOS3_LVO_Name( -750, StrToDate ),
	AOS3_LVO_Name( -756, InternalLoadSeg ),
	AOS3_LVO_Name( -762, InternalUnLoadSeg ),
	AOS3_LVO_Regs( -768, NewLoadSeg ),
	AOS3_LVO_Regs( -774, AddSegment ),
	AOS3_LVO_Name( -780, FindSegment ), // AOS3_Dos_Func_FindSegment },
	AOS3_LVO_Name( -786, RemSegment ),
	AOS3_LVO_Name( -792, CheckSignal ),
	AOS3_LVO_Name( -798, ReadArgs ), // AOS3_Dos_Func_ReadArgs },
	AOS3_LVO_Name( -804, FindArg ), // AOS3_Dos_Func_FindArg },
	AOS3_LVO_Name( -810, ReadItem ),
	AOS3_LVO_Name( -816, StrToLong ), // AOS3_Dos_Func_StrToLong },
	AOS3_LVO_Name( -822, MatchFirst ), // AOS3_Dos_Func_MatchFirst },
	AOS3_LVO_Name( -828, MatchNext ),
	AOS3_LVO_Name( -834, MatchEnd ),
	AOS3_LVO_Name( -840, ParsePattern ), // AOS3_Dos_Func_ParsePattern },
	AOS3_LVO_Name( -846, MatchPattern ), // AOS3_Dos_Func_MatchPattern },
	AOS3_LVO_Name( -858, FreeArgs ),
	AOS3_LVO_Name( -870, FilePart ), // AOS3_Dos_Func_FilePart },
	AOS3_LVO_Name( -876, PathPart ), // AOS3_Dos_Func_PathPart },
	AOS3_LVO_Name( -882, AddPart ), // AOS3_Dos_Func_AddPath },
	AOS3_LVO_Name( -888, StartNotify ),
	AOS3_LVO_Name( -894, EndNotify ),
	AOS3_LVO_Name( -900, SetVar ), // AOS3_Dos_Func_SetVar },
	AOS3_LVO_Name( -906, GetVar ), // AOS3_Dos_Func_GetVar },
	AOS3_LVO_Name( -912, DeleteVar ), // AOS3_Dos_Func_DeleteVar },
	AOS3_LVO_Name( -918, FindVar ), // AOS3_Dos_Func_FindVar },
	AOS3_LVO_Name( -930, CliInitNewcli ),
	AOS3_LVO_Name( -936, CliInitRun ),
	AOS3_LVO_Name( -942, WriteChars ),
	AOS3_LVO_Regs( -948, PutStr ),
	AOS3_LVO_Regs( -954, VPrintf ),
	AOS3_LVO_Regs( -966, ParsePatternNoCase ),
	AOS3_LVO_Regs( -972, MatchPatternNoCase ),
	AOS3_LVO_Name( -984, SameDevice ),
	AOS3_LVO_Name( -990, ExAllEnd ),
	AOS3_LVO_Name( -996, SetOwner ),
	AOS3_LVO_EndMarker()
};

// --
