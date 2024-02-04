
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

static int Global_Exec_Used = false;

struct lvostruct
{
	int16_t	Used;
	int16_t	Offset;
	char *	Name;
	void 	(*Func)( struct M68kStruct *ms );
};

// --

struct myLibType
{
	enum LibType	Type;
	char *			Name;
};

static struct myLibType myLibs[] =
{
{ LIBT_AmigaguideBase,		"amigaguide.library" },
{ LIBT_AslBase,				"asl.library" },
{ LIBT_BulletBase,			"bullet.library" },
{ LIBT_ColorwheelBase,		"colorwheel.library" },
{ LIBT_CommoditiesBase,		"commodities.library" },
{ LIBT_DatatypesBase,		"datatypes.library" },
{ LIBT_DiskfontBase,		"diskfont.library" },
{ LIBT_DosBase,				"dos.library" },
{ LIBT_DTClassBase,			"dtclass.library" },
{ LIBT_ExecBase,			"exec.library" },
{ LIBT_ExpansionBase,		"expansion.library" },
{ LIBT_GadToolsBase,		"gadtools.library" },
{ LIBT_GraphicsBase,		"graphics.library" },
{ LIBT_IconBase,			"icon.library" },
{ LIBT_IFFParseBase,		"iffparse.library" },
{ LIBT_IntuitionBase,		"intuition.library" },
{ LIBT_KeymapBase,			"keymap.library" },
{ LIBT_LayersBase,			"layers.library" },
{ LIBT_LocaleBase,			"locale.library" },
{ LIBT_LowlevelBase,		"lowlevel.library" },
{ LIBT_MathffpBase,			"mathffp.library" },
{ LIBT_MathieeedoubbasBase,		"mathieeedoubbas.library" },
{ LIBT_MathieeedoubtransBase,	"mathieeedoubtrans.library" },
{ LIBT_MathieeesingbasBase,		"mathieeesingbas.library" },
{ LIBT_MathieeesingtransBase,	"mathieeesingtrans.library" },
{ LIBT_MathtransBase,		"mathtrans.library" },
{ LIBT_NonvolatileBase,		"nonvolatile.library" },
{ LIBT_RexxsyslibBase,		"rexxsyslib.library" },
{ LIBT_TranslatorBase,		"translator.library" },
{ LIBT_UtilityBase,			"utility.library" },
{ LIBT_WorkbenchBase,		"wb.library" },
{ 0, NULL }
};

static void Func_OpenLibrary( struct M68kStruct *ms )
{
struct HunkLabel *hl;
char *buf;
int pos;

	// A1 = Library name
	if ( ms->ms_Registers[REG_A1].mr_Type != RT_Label )
	{
		goto bailout;
	}

	hl = ms->ms_Registers[REG_A1].mr_LabelNode;

	if ( hl == NULL )
	{
		goto bailout;
	}

	buf = (void *) hl->hl_Label_Memory;

	if ( buf == NULL )
	{
		goto bailout;
	}

	Mark_NulString( hl );

	pos = 0;

	while( myLibs[pos].Name )
	{
		if ( ! strcmp( buf, myLibs[pos].Name ))
		{
			break;
		}
		else
		{
			pos++;
		}
	}

	if ( myLibs[pos].Name )
	{
		ms->ms_Registers[REG_D0].mr_Type = RT_Library + ( myLibs[pos].Type << 16 );
	}
	else
	{
		ms->ms_Registers[REG_D0].mr_Type = RT_Unknown;

		if ( ms->ms_HunkStruct->hs_PassNr == 1 )
		{
			printf( "Unsupported %s library found at %08x\n", buf, ms->ms_MemoryAdr );
		}
	}

	// Don't clear Reg D0
	ms->ms_ClearRegMask &= ~( 1 << REG_D0 ); 
	
bailout:

	return;
}

// --

static struct myLibType myRes[] =
{
{ LIBT_BattclockBase,	"battclock.resource" },
{ LIBT_BattmemBase,		"battmem.resource" },
{ LIBT_CardresBase,		"cardres.resource " },
{ LIBT_CiaBase,			"ciaa.resource" },
{ LIBT_CiaBase,			"ciab.resource" },
{ LIBT_DiskBase,		"disk.resource " },
{ LIBT_MiscBase,		"misc.resource" },
{ LIBT_PotgoBase,		"potgo.resource" },
{ 0, NULL }
};

static void Func_OpenResource( struct M68kStruct *ms )
{
struct HunkLabel *hl;
char *buf;
int pos;

	// A1 = Resource name
	if ( ms->ms_Registers[REG_A1].mr_Type != RT_Label )
	{
		goto bailout;
	}

	hl = ms->ms_Registers[REG_A1].mr_LabelNode;

	if ( hl == NULL )
	{
		goto bailout;
	}

	buf = (void *) hl->hl_Label_Memory;

	if ( buf == NULL )
	{
		goto bailout;
	}

	Mark_NulString( hl );

	pos = 0;

	while( myRes[pos].Name )
	{
		if ( ! strcmp( buf, myRes[pos].Name ))
		{
			break;
		}
		else
		{
			pos++;
		}
	}

	if ( myRes[pos].Name )
	{
		ms->ms_Registers[REG_D0].mr_Type = RT_Library + ( myRes[pos].Type << 16 );
	}
	else
	{
		ms->ms_Registers[REG_D0].mr_Type = RT_Unknown;

		if ( ms->ms_HunkStruct->hs_PassNr == 1 )
		{
			printf( "Unsupported %s library found at %08x\n", buf, ms->ms_MemoryAdr );
		}
	}

	// Don't clear Reg D0
	ms->ms_ClearRegMask &= ~( 1 << REG_D0 ); 
	
bailout:

	return;
}

// --

static struct lvostruct LVOS[] =
{
	{ 0, -30, "_LVOSupervisor", NULL },
	{ 0, -72, "_LVOInitCode", NULL },
	{ 0, -78, "_LVOInitStruct", NULL },
	{ 0, -84, "_LVOMakeLibrary", NULL },
	{ 0, -90, "_LVOMakeFunctions", NULL },
	{ 0, -96, "_LVOFindResident", Mark_A1_NulString },
	{ 0, -102, "_LVOInitResident", NULL },
	{ 0, -108, "_LVOAlert", NULL },
	{ 0, -114, "_LVODebug", NULL },
	{ 0, -120, "_LVODisable", NULL },
	{ 0, -126, "_LVOEnable", NULL },
	{ 0, -132, "_LVOForbid", NULL },
	{ 0, -138, "_LVOPermit", NULL },
	{ 0, -144, "_LVOSetSR", NULL },
	{ 0, -150, "_LVOSuperState", NULL },
	{ 0, -156, "_LVOUserState", NULL },
	{ 0, -162, "_LVOSetIntVector", NULL },
	{ 0, -168, "_LVOAddIntServer", NULL },
	{ 0, -174, "_LVORemIntServer", NULL },
	{ 0, -180, "_LVOCause", NULL },
	{ 0, -186, "_LVOAllocate", NULL },
	{ 0, -192, "_LVODeallocate", NULL },
	{ 0, -198, "_LVOAllocMem", NULL },
	{ 0, -204, "_LVOAllocAbs", NULL },
	{ 0, -210, "_LVOFreeMem", NULL },
	{ 0, -216, "_LVOAvailMem", NULL },
	{ 0, -222, "_LVOAllocEntry", NULL },
	{ 0, -228, "_LVOFreeEntry", NULL },
	{ 0, -234, "_LVOInsert", NULL },
	{ 0, -240, "_LVOAddHead", NULL },
	{ 0, -246, "_LVOAddTail", NULL },
	{ 0, -252, "_LVORemove", NULL },
	{ 0, -258, "_LVORemHead", NULL },
	{ 0, -264, "_LVORemTail", NULL },
	{ 0, -270, "_LVOEnqueue", NULL },
	{ 0, -276, "_LVOFindName", Mark_A1_NulString },
	{ 0, -282, "_LVOAddTask", NULL },
	{ 0, -288, "_LVORemTask", NULL },
	{ 0, -294, "_LVOFindTask", Mark_A1_NulString },
	{ 0, -300, "_LVOSetTaskPri", NULL },
	{ 0, -306, "_LVOSetSignal", NULL },
	{ 0, -312, "_LVOSetExcept", NULL },
	{ 0, -318, "_LVOWait", NULL },
	{ 0, -324, "_LVOSignal", NULL },
	{ 0, -330, "_LVOAllocSignal", NULL },
	{ 0, -336, "_LVOFreeSignal", NULL },
	{ 0, -342, "_LVOAllocTrap", NULL },
	{ 0, -348, "_LVOFreeTrap", NULL },
	{ 0, -354, "_LVOAddPort", NULL },
	{ 0, -360, "_LVORemPort", NULL },
	{ 0, -366, "_LVOPutMsg", NULL },
	{ 0, -372, "_LVOGetMsg", NULL },
	{ 0, -378, "_LVOReplyMsg", NULL },
	{ 0, -384, "_LVOWaitPort", NULL },
	{ 0, -390, "_LVOFindPort", Mark_A1_NulString },
	{ 0, -396, "_LVOAddLibrary", NULL },
	{ 0, -402, "_LVORemLibrary", NULL },
	{ 0, -408, "_LVOOldOpenLibrary", Func_OpenLibrary },
	{ 0, -414, "_LVOCloseLibrary", NULL },
	{ 0, -420, "_LVOSetFunction", NULL },
	{ 0, -426, "_LVOSumLibrary", NULL },
	{ 0, -432, "_LVOAddDevice", NULL },
	{ 0, -438, "_LVORemDevice", NULL },
	{ 0, -444, "_LVOOpenDevice", Mark_A0_NulString },
	{ 0, -450, "_LVOCloseDevice", NULL },
	{ 0, -456, "_LVODoIO", NULL },
	{ 0, -462, "_LVOSendIO", NULL },
	{ 0, -468, "_LVOCheckIO", NULL },
	{ 0, -474, "_LVOWaitIO", NULL },
	{ 0, -480, "_LVOAbortIO", NULL },
	{ 0, -486, "_LVOAddResource", NULL },
	{ 0, -492, "_LVORemResource", NULL },
	{ 0, -498, "_LVOOpenResource", Func_OpenResource },
	{ 0, -522, "_LVORawDoFmt", Mark_A0_NulString },
	{ 0, -528, "_LVOGetCC", NULL },
	{ 0, -534, "_LVOTypeOfMem", NULL },
	{ 0, -540, "_LVOProcure", NULL },
	{ 0, -546, "_LVOVacate", NULL },
	{ 0, -552, "_LVOOpenLibrary", Func_OpenLibrary },
	{ 0, -558, "_LVOInitSemaphore", NULL },
	{ 0, -564, "_LVOObtainSemaphore", NULL },
	{ 0, -570, "_LVOReleaseSemaphore", NULL },
	{ 0, -576, "_LVOAttemptSemaphore", NULL },
	{ 0, -582, "_LVOObtainSemaphoreList", NULL },
	{ 0, -588, "_LVOReleaseSemaphoreList", NULL },
	{ 0, -594, "_LVOFindSemaphore", Mark_A1_NulString },
	{ 0, -600, "_LVOAddSemaphore", NULL },
	{ 0, -606, "_LVORemSemaphore", NULL },
	{ 0, -612, "_LVOSumKickData", NULL },
	{ 0, -618, "_LVOAddMemList", Mark_A1_NulString },
	{ 0, -624, "_LVOCopyMem", NULL },
	{ 0, -630, "_LVOCopyMemQuick", NULL },
	{ 0, -636, "_LVOCacheClearU", NULL },
	{ 0, -642, "_LVOCacheClearE", NULL },
	{ 0, -648, "_LVOCacheControl", NULL },
	{ 0, -654, "_LVOCreateIORequest", NULL },
	{ 0, -660, "_LVODeleteIORequest", NULL },
	{ 0, -666, "_LVOCreateMsgPort", NULL },
	{ 0, -672, "_LVODeleteMsgPort", NULL },
	{ 0, -678, "_LVOObtainSemaphoreShared", NULL },
	{ 0, -684, "_LVOAllocVec", NULL },
	{ 0, -690, "_LVOFreeVec", NULL },
	{ 0, -696, "_LVOCreatePool", NULL },
	{ 0, -702, "_LVODeletePool", NULL },
	{ 0, -708, "_LVOAllocPooled", NULL },
	{ 0, -714, "_LVOFreePooled", NULL },
	{ 0, -720, "_LVOAttemptSemaphoreShared", NULL },
	{ 0, -726, "_LVOColdReboot", NULL },
	{ 0, -732, "_LVOStackSwap", NULL },
	{ 0, -738, "_LVOChildFree", NULL },
	{ 0, -744, "_LVOChildOrphan", NULL },
	{ 0, -750, "_LVOChildStatus", NULL },
	{ 0, -756, "_LVOChildWait", NULL },
	{ 0, -762, "_LVOCachePreDMA", NULL },
	{ 0, -768, "_LVOCachePostDMA", NULL },
	{ 0, -774, "_LVOAddMemHandler", NULL },
	{ 0, -780, "_LVORemMemHandler", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Exec( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Exec_Used = true;
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
printf( "Unknown Exec libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Exec\n";

int Save_Lib_Exec( void )
{
int error;
int pos;
int len;

	if ( Global_Exec_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Exec lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Exec Data\n", __FILE__, __LINE__ );
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
