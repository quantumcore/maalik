#ifndef __FHDAWN__H__
#define __FHDAWN__H__


/*
Author: Fahad (QuantumCore)
fhdawn.h (c) 2020
Desc: Main header file 
Created:  2020-08-15T15:27:04.427Z
Modified: -
*/

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#define BUFFER 1024

static BOOL connected = FALSE;

struct sockaddr_in server;
WSASocket sockfd;
char recvbuf[BUFFER] = { 0 };


//=====================
void ReportError(void);
void WSAReportError(void);

//=====================
int sockprintf(WSASocket sock, const char* words, ...);

// Start Winsock
int StartWSA()
{
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    {
        printf("[Error] Error Starting Winsock.");
        WSAReportError();
    }
}

void fhdawn_main(void);
void MainConnect(void);

void ExecSock(void);

#endif  //!__FHDAWN__H__