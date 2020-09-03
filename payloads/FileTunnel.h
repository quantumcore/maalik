#ifndef FILE_TUNNEL_
#define FILE_TUNNEL_

// TODO: Switch from fopen(); to CreateFile();
// Update this 

#include <Windows.h>
#include <stdio.h>

#define OUTPUTFILE "output.png" // This is a text file

void WriteOutput(const char* output)
{
    DWORD dwBytesWritten = 0;
    HANDLE FILE = CreateFile((LPSTR)OUTPUTFILE, FILE_WRITE_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(FILE, output, strlen(output), &dwBytesWritten, NULL);
    CloseHandle(FILE);
}


#endif // !FILE_TUNNEL_
