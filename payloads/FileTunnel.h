#ifndef FILE_TUNNEL_
#define FILE_TUNNEL_

// TODO: Switch from fopen(); to CreateFile();
// Update this 

#include <Windows.h>
#include <stdio.h>

#define OUTPUTFILE "output.png" // This is a text file

void WriteOutput(const char* output)
{
    FILE * fs;
    if ((fs = fopen(OUTPUTFILE, "wb")) != NULL)
    {
        fwrite(output, sizeof(output), 1, fs);
        fclose(fs);
    }
}


#endif // !FILE_TUNNEL_
