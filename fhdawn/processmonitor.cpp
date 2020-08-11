#include "fhdawn.h"

// Simple Process MOnitor
void FHDAWN::ProcessMonitor(std::string Process)
{
    const char* px = Process.c_str();
    std::ostringstream rp;
    HANDLE phandle = NULL;
    char chck[BUFFER] = { 0 } ;
    char FILEPATH[MAX_PATH];
    while(true)
    {
        memset(chck, '\0', BUFFER);
        int return_code = recv(sockfd, chck, BUFFER, 0);
        if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
        {
            connected = false;
        }

        if(strcmp(chck, "procstop") == 0)
        {
            break;
        } else {
            rp.clear(); rp.str(""); 
            DWORD pid = ProcessId(px);
            if(pid != 0)
            {
                memset(FILEPATH, '\0', MAX_PATH);
                phandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
                if (phandle != NULL) {
                    if (GetModuleFileNameEx(phandle, NULL, FILEPATH, MAX_PATH) == 0) {
                        rp << "[Process Manager] Process ( " << px << " ) running at PID : " << pid << ", Failed to get Path [ ERROR : " << GetLastError() << " ]";
                        send_data(rp.str()); 
                    } else {
                        rp << "[Process Manager] Process ( " << px << " ) running at PID : " << pid << ", Full Path : " << FILEPATH;
                        send_data(rp.str()); 
                    }
                    CloseHandle(phandle);
                } else {
                    rp << "[Process Manager] Process ( " << px << " ) running at PID : " << pid << ", Failed to get Path [ ERROR : " << GetLastError() << " ]";
                    send_data(rp.str()); 
                }
                Sleep(5000);
            } else {
                send_data("Process not running.");
                break;
            }
        }
    }    
}