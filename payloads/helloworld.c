#include <windows.h>

DWORD WINAPI X()
{
	MessageBoxA(NULL, "Hello World!", "Hello to this World!", MB_ICONINFORMATION);
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH){
		CreateThread(NULL, 0, X, NULL, 0, NULL);
	}

	return TRUE;
}

// int main()
// {
//     MessageBoxA(NULL, "Hello World!", "Hello to this World!", MB_ICONINFORMATION);
// }