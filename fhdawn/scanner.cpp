#include "fhdawn.h"

std::string FHDAWN::IP2Host(const char* IP)
{
    struct in_addr destip;
    struct hostent* info;

    destip.s_addr = inet_addr(IP);
    info = gethostbyaddr((char*)&destip, sizeof(destip), AF_INET);
    if(info == NULL)
    {
        return "NULL";    
    } else {
        return std::string(info->h_name);
    }
}


void FHDAWN::CheckHost(const char* ip_address)
{
	DWORD ret;
	IPAddr srcip = 0;
	ULONG MacAddr[2];
    std::ostringstream err_response;
	ULONG PhyAddrLen = 6;  
	unsigned char mac[6];
	struct in_addr destip;
	destip.s_addr = inet_addr(ip_address);
    char MacAddress[200];
	int i;
    memset(mac, '\0', 6);
    memset(MacAddress, '\0', 200);
	ret = SendARP((IPAddr) destip.S_un.S_addr , srcip , MacAddr , &PhyAddrLen);
	if(ret == NO_ERROR)
    {
        if(PhyAddrLen)
        {
            BYTE *bMacAddr = (BYTE *) & MacAddr;
            for (i = 0; i < (int) PhyAddrLen; i++)
            {
                mac[i] = (char)bMacAddr[i];
            }

            snprintf(MacAddress, 200, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
            std::string Result = "[HOST]" + std::string(ip_address) + " - " + IP2Host(ip_address) + " - " + std::string(MacAddress);
            send_data(Result);
        } else {
            send_data("Warning: SendArp completed successfully, but returned 0. Failed to get MAC.");
        }
    } else {    
        err_response << "Error Failed to get MAC : " << ip_address << "\n";
        send_data(err_response.str());
    }
}


void FHDAWN::checkPort(const char* ip, int port)
{
    SOCKET connectsock;
    struct sockaddr_in hostx;
    std::ostringstream err;
    // u_long mode = 1;  
    connectsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
    if(connectsock == SOCKET_ERROR || connectsock == INVALID_SOCKET)
    {
        err << "Error creating socket : " << WSAGetLastError();
        send_data(err.str());
    }
    // ioctlsocket(connectsock, FIONBIO, &mode);

    hostx.sin_addr.s_addr = inet_addr(ip);
    hostx.sin_port = htons(port);
    hostx.sin_family = AF_INET;

    int check = connect(connectsock, (struct sockaddr*)&hostx, sizeof(hostx));
    if(check != SOCKET_ERROR)
    {
        err << ip << ","<<port;
        send_data("OPENPORT:" + err.str());
        closesocket(connectsock);
        
    } else {
        closesocket(connectsock);
    }
    
}