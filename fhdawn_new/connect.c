/*
Author: Fahad (QuantumCore)
fhdawn.h (c) 2020
Desc: Main header file 
Created:  2020-08-15T15:27:04.427Z
Modified: -
*/

#include "fhdawn.h"

int sockprintf(SOCKET sock, const char* words, ...) {
    static char textBuffer[BUFFER];
    va_list args;
    va_start(args, words);
    vsprintf(textBuffer, words, args);
    va_end(args);
    return send(sock, textBuffer, strlen(textBuffer), NULL); // see, it's printf but for a socket. instead of printing, at the end it's a send()
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