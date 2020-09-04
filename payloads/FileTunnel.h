#ifndef FILE_TUNNEL_
#define FILE_TUNNEL_

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

char* GetInputOutput()
{
    char * buffer = 0;
    long length;
    FILE * f = fopen (OUTPUTFILE, "rb");

    if (f)
    {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = malloc (length);
    if (buffer)
    {
        fread (buffer, 1, length, f);
    }
    fclose (f);
    }

    if (buffer)
    {
        return buffer;
    }
}

#endif // !FILE_TUNNEL_
