// pgputenv_demo.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

PG_MODULE_MAGIC;

Datum __declspec(dllexport) pgputenv_depart(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(pgputenv_depart);

HMODULE ghModule;

Datum pgputenv_depart(PG_FUNCTION_ARGS)
{
	/* Get path to this DLL and add to DLL search path. */
	WCHAR myPath[MAX_PATH + 1];
	memset(myPath, 0, sizeof(myPath));
	if (!GetModuleFileName(ghModule, myPath, MAX_PATH))
	{
		PG_RETURN_INT32(1);
	}

	PWSTR lastSep = wcsrchr(myPath, '\\');
	if (lastSep > myPath)
	{
		*lastSep = 0;
	}
	else
	{
		PG_RETURN_INT32(2);
	}
	SetDllDirectory(myPath);

	HMODULE dll = LoadLibrary(L"indirect");
	if (dll)
	{
		/* Set variable to let pgwin32_putenv() detect indirect's CRT. */
		putenv("PGPUTENV_DEPART=PHASE_1");
		OutputDebugString(L"Loaded indirect.dll\n");

		/* Unload indirect, thereby unloading its CRT. */
		FreeLibrary(dll);
		OutputDebugString(L"Freed indirect.dll\n");

		/* Set another variable to make pgwin32_putenv() crash. */
		putenv("PGPUTENV_DEPART=PHASE_2");
		OutputDebugString(L"I AM ALIVE!\n"); /* No, I'm not. */
	}
	
	PG_RETURN_INT32(4711);
}