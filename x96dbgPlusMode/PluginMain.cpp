#include "PluginMain.h"
#include "Plugin.h"
#include "AttackVMP.h"

int pluginHandle;
HWND hwndDlg;
int hMenu;
int hMenuDisasm;
int hMenuDump;
int hMenuStack;
HINSTANCE hInstance;

PLUG_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct)
{
	initStruct->pluginVersion = PLUGIN_VERSION;
	initStruct->sdkVersion = PLUG_SDKVERSION;
	strncpy_s(initStruct->pluginName, PLUGIN_NAME, _TRUNCATE);
	pluginHandle = initStruct->pluginHandle;
	CAttackVMP::GetInstance().pluginHandle = pluginHandle;
	return pluginInit(initStruct);
}

PLUG_EXPORT bool plugstop()
{
	return pluginStop();
}

PLUG_EXPORT void plugsetup(PLUG_SETUPSTRUCT* setupStruct)
{
	hwndDlg = setupStruct->hwndDlg;
	hMenu = setupStruct->hMenu;
	hMenuDisasm = setupStruct->hMenuDisasm;
	hMenuDump = setupStruct->hMenuDump;
	hMenuStack = setupStruct->hMenuStack;
	CAttackVMP::GetInstance().hMenu = hMenu;
	CAttackVMP::GetInstance().hwndDlg = hwndDlg;
	CAttackVMP::GetInstance().hMenuDisasm = hMenuDisasm;
	CAttackVMP::GetInstance().hMenuDump = hMenuDump;
	CAttackVMP::GetInstance().hMenuStack = hMenuStack;

	pluginSetup();
}



BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		hInstance = hinstDLL; // save instance handle for future use
		CAttackVMP::GetInstance().hInstance = hinstDLL;
	}
	return TRUE;
}
