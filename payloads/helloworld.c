#include <windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH){
		MessageBoxA(NULL, "Hello World!", "Hello to this World!", MB_ICONINFORMATION);
	}

	return TRUE;
}

// int main()
// {
//     MessageBoxA(NULL, "Hello World!", "Hello to this World!", MB_ICONINFORMATION);
// }