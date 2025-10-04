
/*
** Copyright (c) 2014-2025 Rene W. Olsen
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
** This software is released under the GNU General Public License, version 3.
** For the full text of the license, please visit:
** https://www.gnu.org/licenses/gpl-3.0.html
**
** You can also find a copy of the license in the LICENSE file included with this software.
*/

// --

#include "Resourcer/ReSrc4.h"

// --
// Single Arg

static AOS3_RegStruct _String_A0[] =
{
	AOS3_REG_String( M68KREGT_A0 ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _String_A1[] =
{
	AOS3_REG_String( M68KREGT_A1 ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _MsgPort_A1[] =
{
	AOS3_REG_Struct( M68KREGT_A1, RS4StructID_MsgPort ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _Code_A5[] =
{
	AOS3_REG_Code( M68KREGT_A5 ),
	AOS3_REG_EndMarker()
};

// --

static AOS3_RegStruct _Interrupt_A1[] =
{
	AOS3_REG_Struct( M68KREGT_A1, RS4StructID_Interrupt ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _StackSwap_A0[] =
{
	AOS3_REG_Struct( M68KREGT_A0, RS4StructID_StackSwap ),
	AOS3_REG_EndMarker()
};


// --
// Single Args

#define AOS3_OpenDevice_Regs		_String_A0

#define AOS3_FindName_Regs			_String_A1
#define AOS3_FindTask_Regs			_String_A1
#define AOS3_AddMemList_Regs		_String_A1

#define AOS3_AddPort_Regs			_MsgPort_A1

#define AOS3_AddIntServer_Regs		_Interrupt_A1
#define AOS3_RemIntServer_Regs		_Interrupt_A1

#define AOS3_StackSwap_Regs			_StackSwap_A0

#define AOS3_Supervisor_Regs		_Code_A5

// --
// Functions

#define AOS3_OpenLibrary_Func		AOS3_Exec_OpenLibrary_Func
#define AOS3_OldOpenLibrary_Func	AOS3_Exec_OpenLibrary_Func
#define AOS3_FindPort_Func			AOS3_Exec_FindPort_Func
#define AOS3_FindResident_Func		AOS3_Exec_FindResident_Func
#define AOS3_FindSemaphore_Func		AOS3_Exec_FindSemaphore_Func
#define AOS3_OpenResource_Func		AOS3_Exec_OpenResource_Func

// --

enum RS4DecodeStat AOS3_Exec_OpenResource_Func( enum RS4ErrorCode *errcode, RS4Trace *rt );
enum RS4DecodeStat AOS3_Exec_OpenLibrary_Func( enum RS4ErrorCode *errcode, RS4Trace *rt );
enum RS4DecodeStat AOS3_Exec_FindPort_Func( enum RS4ErrorCode *errcode, RS4Trace *rt );
enum RS4DecodeStat AOS3_Exec_FindResident_Func( enum RS4ErrorCode *errcode, RS4Trace *rt );
enum RS4DecodeStat AOS3_Exec_FindSemaphore_Func( enum RS4ErrorCode *errcode, RS4Trace *rt );

AOS3_LVOStruct AOS3_ExecBase[] =
{
	AOS3_LVO_Regs( -30, Supervisor ),
	AOS3_LVO_Name( -72, InitCode ),
	AOS3_LVO_Name( -78, InitStruct ),
	AOS3_LVO_Name( -84, MakeLibrary ),
	AOS3_LVO_Name( -90, MakeFunctions ),
	AOS3_LVO_Func( -96, FindResident ),
	AOS3_LVO_Name( -102, InitResident ),
	AOS3_LVO_Name( -108, Alert ),
	AOS3_LVO_Name( -114, Debug ),
	AOS3_LVO_Name( -120, Disable ),
	AOS3_LVO_Name( -126, Enable ),
	AOS3_LVO_Name( -132, Forbid ),
	AOS3_LVO_Name( -138, Permit ),
	AOS3_LVO_Name( -144, SetSR ),
	AOS3_LVO_Name( -150, SuperState ),
	AOS3_LVO_Name( -156, UserState ),
	AOS3_LVO_Name( -162, SetIntVector ),
	AOS3_LVO_Regs( -168, AddIntServer ),
	AOS3_LVO_Regs( -174, RemIntServer ),
	AOS3_LVO_Name( -180, Cause ),
	AOS3_LVO_Name( -186, Allocate ),
	AOS3_LVO_Name( -192, Deallocate ),
	AOS3_LVO_Name( -198, AllocMem ),
	AOS3_LVO_Name( -204, AllocAbs ),
	AOS3_LVO_Name( -210, FreeMem ),
	AOS3_LVO_Name( -216, AvailMem ),
	AOS3_LVO_Name( -222, AllocEntry ),
	AOS3_LVO_Name( -228, FreeEntry ),
	AOS3_LVO_Name( -234, Insert ),
	AOS3_LVO_Name( -240, AddHead ),
	AOS3_LVO_Name( -246, AddTail ),
	AOS3_LVO_Name( -252, Remove ),
	AOS3_LVO_Name( -258, RemHead ),
	AOS3_LVO_Name( -264, RemTail ),
	AOS3_LVO_Name( -270, Enqueue ),
	AOS3_LVO_Regs( -276, FindName ),
	AOS3_LVO_Name( -282, AddTask ),
	AOS3_LVO_Name( -288, RemTask ),
	AOS3_LVO_Regs( -294, FindTask ),
	AOS3_LVO_Name( -300, SetTaskPri ),
	AOS3_LVO_Name( -306, SetSignal ),
	AOS3_LVO_Name( -312, SetExcept ),
	AOS3_LVO_Name( -318, Wait ),
	AOS3_LVO_Name( -324, Signal ),
	AOS3_LVO_Name( -330, AllocSignal ),
	AOS3_LVO_Name( -336, FreeSignal ),
	AOS3_LVO_Name( -342, AllocTrap ),
	AOS3_LVO_Name( -348, FreeTrap ),
	AOS3_LVO_Regs( -354, AddPort ),
	AOS3_LVO_Name( -360, RemPort ),
	AOS3_LVO_Name( -366, PutMsg ),
	AOS3_LVO_Name( -372, GetMsg ),
	AOS3_LVO_Name( -378, ReplyMsg ),
	AOS3_LVO_Name( -384, WaitPort ),
	AOS3_LVO_Func( -390, FindPort ),
	AOS3_LVO_Name( -396, AddLibrary ),
	AOS3_LVO_Name( -402, RemLibrary ),
	AOS3_LVO_Func( -408, OldOpenLibrary ),
	AOS3_LVO_Name( -414, CloseLibrary ),
	AOS3_LVO_Name( -420, SetFunction ),
	AOS3_LVO_Name( -426, SumLibrary ),
	AOS3_LVO_Name( -432, AddDevice ),
	AOS3_LVO_Name( -438, RemDevice ),
	AOS3_LVO_Regs( -444, OpenDevice ),
	AOS3_LVO_Name( -450, CloseDevice ),
	AOS3_LVO_Name( -456, DoIO ),
	AOS3_LVO_Name( -462, SendIO ),
	AOS3_LVO_Name( -468, CheckIO ),
	AOS3_LVO_Name( -474, WaitIO ),
	AOS3_LVO_Name( -480, AbortIO ),
	AOS3_LVO_Name( -486, AddResource ),
	AOS3_LVO_Name( -492, RemResource ),
	AOS3_LVO_Func( -498, OpenResource ), //, AOS3_Exec_Func_OpenResource },
	AOS3_LVO_Name( -522, RawDoFmt ), //, AOS3_Exec_Func_DoRawFmt },
	AOS3_LVO_Name( -528, GetCC ),
	AOS3_LVO_Name( -534, TypeOfMem ),
	AOS3_LVO_Name( -540, Procure ),
	AOS3_LVO_Name( -546, Vacate ),
	AOS3_LVO_Func( -552, OpenLibrary ),
	AOS3_LVO_Name( -558, InitSemaphore ),
	AOS3_LVO_Name( -564, ObtainSemaphore ),
	AOS3_LVO_Name( -570, ReleaseSemaphore ),
	AOS3_LVO_Name( -576, AttemptSemaphore ),
	AOS3_LVO_Name( -582, ObtainSemaphoreList ),
	AOS3_LVO_Name( -588, ReleaseSemaphoreList ),
	AOS3_LVO_Func( -594, FindSemaphore ),
	AOS3_LVO_Name( -600, AddSemaphore ), 
	AOS3_LVO_Name( -606, RemSemaphore ),
	AOS3_LVO_Name( -612, SumKickData ),
	AOS3_LVO_Regs( -618, AddMemList ),
	AOS3_LVO_Name( -624, CopyMem ),
	AOS3_LVO_Name( -630, CopyMemQuick ),
	AOS3_LVO_Name( -636, CacheClearU ),
	AOS3_LVO_Name( -642, CacheClearE ),
	AOS3_LVO_Name( -648, CacheControl ),
	AOS3_LVO_Name( -654, CreateIORequest ),
	AOS3_LVO_Name( -660, DeleteIORequest ),
	AOS3_LVO_Name( -666, CreateMsgPort ),
	AOS3_LVO_Name( -672, DeleteMsgPort ),
	AOS3_LVO_Name( -678, ObtainSemaphoreShared ),
	AOS3_LVO_Name( -684, AllocVec ),
	AOS3_LVO_Name( -690, FreeVec ),
	AOS3_LVO_Name( -696, CreatePool ),
	AOS3_LVO_Name( -702, DeletePool ),
	AOS3_LVO_Name( -708, AllocPooled ),
	AOS3_LVO_Name( -714, FreePooled ),
	AOS3_LVO_Name( -720, AttemptSemaphoreShared ),
	AOS3_LVO_Name( -726, ColdReboot ),
	AOS3_LVO_Regs( -732, StackSwap ),
	AOS3_LVO_Name( -738, ChildFree ),
	AOS3_LVO_Name( -744, ChildOrphan ),
	AOS3_LVO_Name( -750, ChildStatus ),
	AOS3_LVO_Name( -756, ChildWait ),
	AOS3_LVO_Name( -762, CachePreDMA ),
	AOS3_LVO_Name( -768, CachePostDMA ),
	AOS3_LVO_Name( -774, AddMemHandler ),
	AOS3_LVO_Name( -780, RemMemHandler ),
	AOS3_LVO_EndMarker()
};

// --
