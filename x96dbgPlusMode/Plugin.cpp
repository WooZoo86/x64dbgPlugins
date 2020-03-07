#include "Plugin.h"
#include "PluginMain.h"
#include "AttackVMP.h"
#include "x96CallbackFunction.h"
#define PLOG(Format, ...)                   _plugin_logprintf(Format, __VA_ARGS__)
#define RVA_TO_ADDR_OR_ZERO(Mapping,Rva)    (Rva > 0) ? (RVA_TO_ADDR(Mapping,Rva)) : (0)


bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
	typedef bool CommandCallback_t(int, char**);
	
	auto RegisterRequiredCommand = [](const char* CommandString, CommandCallback_t CommandCallback)
	{
		if (!_plugin_registercommand(pluginHandle, CommandString, CommandCallback, true))
		{
			PLOG("[" PLUGIN_NAME "]  failed to register command %s.\n", CommandString);
			return false;
		}
		return true;
	};
	_plugin_registercallback(pluginHandle, CB_MENUENTRY, (CBPLUGIN)cbMenuCallback);//注册菜单的回调函数
	//_plugin_registercallback(pluginHandle, CB_MENUENTRY, (CBPLUGIN)CBINITDEBUG);//注册SDK CBINITDEBUG 回调函数
	/*
	if (!RegisterRequiredCommand(cmdDumpPEHeader, cbDumpPEHeader)) return false;
	if (!RegisterRequiredCommand(cmdDumpDOSHeader, cbDumpDOSHeader)) return false;
	if (!RegisterRequiredCommand(cmdDumpNTHeaders, cbDumpNTHeaders)) return false;
	if (!RegisterRequiredCommand(cmdDumpFileHeader, cbDumpFileHeader)) return false;
	if (!RegisterRequiredCommand(cmdDumpOptionalHeader, cbDumpOptionalHeader)) return false;
	if (!RegisterRequiredCommand(cmdDumpDataDirectories, cbDumpDataDirectories)) return false;
	if (!RegisterRequiredCommand(cmdDumpSections, cbDumpSections)) return false;
	*/
	return true;
}

bool pluginStop()
{
	_plugin_menuclear(hMenu);
	/*
	_plugin_unregistercommand(pluginHandle, cmdDumpPEHeader);
	_plugin_unregistercommand(pluginHandle, cmdDumpDOSHeader);
	_plugin_unregistercommand(pluginHandle, cmdDumpNTHeaders);
	_plugin_unregistercommand(pluginHandle, cmdDumpFileHeader);
	_plugin_unregistercommand(pluginHandle, cmdDumpOptionalHeader);
	_plugin_unregistercommand(pluginHandle, cmdDumpDataDirectories);
	_plugin_unregistercommand(pluginHandle, cmdDumpSections);
	*/
	return true;
}

void pluginSetup()
{
	_plugin_menuaddentry(hMenu, MenuList::PLUGIN_MENU_ANALYSIS, "&Analysis");//添加菜单标签
	_plugin_menuaddentry(hMenu, MenuList::PLUGIN_MENU_ABOUT, "&About");//添加菜单标签
}