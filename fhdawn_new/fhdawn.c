/*
Author: Fahad (QuantumCore)
fhdawn.c (c) 2020
Created:  2020-08-15T15:27:04.427Z
Modified: -
*/

#include "fhdawn.h"

void ReportError(void)
{
    printf("Error : %ld\n", GetLastError());
}

void WSAReportError(void)
{
    printf("Error : %ld\n", WSAGetLastError());
}

void split(char* src, char* dest[5], const char* delimeter) {
	int i = 0;
	char *p = strtok(src, delimeter);
	while (p != NULL)
	{
		dest[i++] = p;
		p = strtok(NULL, delimeter);
	}
}

DWORD ProcessId(LPCTSTR ProcessName)
{
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt)) {
        do {
            if (!lstrcmpi(pt.szExeFile, ProcessName)) {
                CloseHandle(hsnap);
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
    }
    CloseHandle(hsnap);
    return 0;
}