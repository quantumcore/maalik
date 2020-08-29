#include <windows.h>


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH){
		char buf[]  = " I am exe, in memory, my execution, is fantasitc, you can build me stattic, and run me anywhereee, imagination, that is your creation.\n";
        DWORD written = 0;
        HANDLE x= CreateFile("MEM.txt", FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        WriteFile(x, buf, sizeof(buf), &written, NULL);
        CloseHandle(x);
        system("notepad MEM.txt");
	}

	return TRUE;
}