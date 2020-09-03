#ifndef FILE_TUNNEL_
#define FILE_TUNNEL_

#include "fhdawn.h"

#define OUTPUTFILE "output.png" // This is a text file

// Update this

//char* GetOutputData()
//{
//	FILE* f = fopen(OUTPUTFILE, "rb");
//	fseek(f, 0, SEEK_END);
//	long fsize = ftell(f);
//	fseek(f, 0, SEEK_SET);
//
//	char* string = (char*)malloc(fsize + 1);
//	fread(string, 1, fsize, f);
//	fclose(f);
//	string[fsize] = 0;
//	return string;
//}
//

#endif // !FILE_TUNNEL_
