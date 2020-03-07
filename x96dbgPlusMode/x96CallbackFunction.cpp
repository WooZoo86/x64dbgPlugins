#include "x96CallbackFunction.h"
#include "PluginMain.h"
#include "AttackVMP.h"
#include "Plugin.h"
using namespace Script;



void CBINITDEBUG(CBTYPE bType, PLUG_CB_INITDEBUG *callbackInfo) /* 初始化调试器回调函数 */
{
	PLOG("Attack VMP : InitDebug\n");
}

void CBSTOPDEBUG(CBTYPE bType, PLUG_CB_STOPDEBUG * callbackInfo)
{
	PLOG("Attack VMP : CBSTOPDEBUG\n");
	return ;
}

void CBCREATEPROCESS(CBTYPE bType, PLUG_CB_CREATEPROCESS * callbackInfo)
{
	  PLOG("Attack VMP : CBCREATEPROCESS\n");
	 return;
}

void CBEXITPROCESS(CBTYPE bType, PLUG_CB_EXITPROCESS * callbackInfo)
{
	  PLOG("Attack VMP : CBEXITPROCESS\n");
	return ;
}

void CBEXITTHREAD(CBTYPE bType, PLUG_CB_CREATETHREAD * callbackInfo)
{
	  PLOG("Attack VMP : CBEXITTHREAD\n");
	return ;
}



void CBSYSTEMBREAKPOINT(CBTYPE bType, PLUG_CB_SYSTEMBREAKPOINT * callbackInfo) 
{
	CAttackVMP::GetInstance().MainModuleBase = Module::GetMainModuleBase();
	CAttackVMP::GetInstance().MainModuleSize = Module::GetMainModuleSize();
	CAttackVMP::GetInstance().MainModuleEntry = Module::GetMainModuleEntry();
	CAttackVMP::GetInstance().analysisblock.clear();
	CAttackVMP::GetInstance().analysisblock.emplace_back(Block{ Module::GetMainModuleBase() , Module::GetMainModuleSize() });
	PLOG("MainModuleBase : %x, MainModuleSize : %x, MainModuleEntry : %x\n", CAttackVMP::GetInstance().MainModuleBase, CAttackVMP::GetInstance().MainModuleSize, CAttackVMP::GetInstance().MainModuleEntry);

}

void CBLOADDLL(CBTYPE bType, PLUG_CB_LOADDLL * callbackInfo)
{
	  PLOG("Attack VMP : CBLOADDLL\n");
	  return ;
}

void CBUNLOADDLL(CBTYPE bType, PLUG_CB_UNLOADDLL * callbackInfo)
{
	PLOG("Attack VMP : CBUNLOADDLL\n");
	return ;
}

void CBOUTPUTDEBUGSTRING(CBTYPE bType, PLUG_CB_OUTPUTDEBUGSTRING * callbackInfo)
{
	PLOG("Attack VMP : CBOUTPUTDEBUGSTRING\n");
	return ;
}

void CBEXCEPTION(CBTYPE bType, PLUG_CB_EXCEPTION * callbackInfo)
{
	PLOG("Attack VMP : CBEXCEPTION\n");
	return ;
}

void CBBREAKPOINT(CBTYPE bType, PLUG_CB_BREAKPOINT * callbackInfo)
{
	PLOG("Attack VMP : CBBREAKPOINT\n");
	return ;
}

void CBPAUSEDEBUG(CBTYPE bType, PLUG_CB_PAUSEDEBUG * callbackInfo)
{
	PLOG("Attack VMP : CBPAUSEDEBUG\n");
	return ;
}

void CBRESUMEDEBUG(CBTYPE bType, PLUG_CB_RESUMEDEBUG * callbackInfo)
{
	PLOG("Attack VMP : CBRESUMEDEBUG\n");
	return ;
}

void CBSTEPPED(CBTYPE bType, PLUG_CB_STEPPED * callbackInfo)
{
	PLOG("Attack VMP : CBSTEPPED\n");
	return ;
}

void CBATTACH(CBTYPE bType, PLUG_CB_ATTACH * callbackInfo)
{
	PLOG("Attack VMP : CBATTACH\n");
	return ;
}

void CBDETACH(CBTYPE bType, PLUG_CB_DETACH * callbackInfo)
{
	PLOG("Attack VMP : CBDETACH\n");
	return ;
}

void CBDEBUGEVENT(CBTYPE bType, PLUG_CB_DEBUGEVENT * callbackInfo)
{
	PLOG("Attack VMP : CBDEBUGEVENT\n");
	return ;
}

void CBMENUENTRY(CBTYPE bType, PLUG_CB_MENUENTRY * callbackInfo)
{
	PLOG("Attack VMP : CBMENUENTRY\n");
	BridgeCFNodeList *list;
	BridgeCFGraphList * graph = new BridgeCFGraphList;
	switch (callbackInfo->hEntry)
	{
	case MenuList::PLUGIN_MENU_ABOUT:
		MessageBoxA(hwndDlg, "Attack VMP", "About", MB_OK + MB_ICONINFORMATION);
		break;
	case MenuList::PLUGIN_MENU_ANALYSIS:


		DbgAnalyzeFunction(0x7ff8dd227b30,graph);

		list = reinterpret_cast<BridgeCFNodeList*>(graph->nodes.data);
		PLOG("%lld\n", list[1].start);

		GuiLoadGraph(graph, 0x7ff8dd227b30);


		//CAttackVMP::GetInstance().Reset();
		//CAttackVMP::GetInstance().StaticAnalysis();
	default:
		break;
	}
	delete graph;
	return ;
}

void CBWINEVENT(CBTYPE bType, PLUG_CB_WINEVENT * callbackInfo)
{
	//PLOG("Attack VMP : CBWINEVENT\n");
	return ;
}

void CBWINEVENTGLOBAL(CBTYPE bType, PLUG_CB_WINEVENTGLOBAL * callbackInfo)
{
	//PLOG("Attack VMP : CBWINEVENTGLOBAL\n");
	return ;
}

void CBSAVEDB(CBTYPE bType, PLUG_CB_LOADSAVEDB * callbackInfo)
{
	PLOG("Attack VMP : CBSAVEDB\n");
	return ;
}
