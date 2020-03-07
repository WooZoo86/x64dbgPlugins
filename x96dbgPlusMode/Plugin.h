#pragma once
#include <Windows.h>
#include "pluginsdk/_plugins.h" 


#define PLUGIN_NAME "Attack VMP"
#define PLUGIN_VERSION 1

bool pluginInit(PLUG_INITSTRUCT* initStruct);
bool pluginStop();
void pluginSetup();



enum MenuList :int
{
	PLUGIN_MENU_ABOUT,
	PLUGIN_MENU_ANALYSIS
};