/*
Author: Fahad (QuantumCore)
fhdawn.c (c) 2020
Created:  2020-08-15T15:27:04.427Z
Modified: -
*/

#include "fhdawn.h"

void ReportError(void)
{
    printf("Error : %ld\n", GetLastError());
}

void WSAReportError(void)
{
    printf("Error : %ld\n", WSAGetLastError());
}

void split(char* src, char* dest[5], const char* delimeter) {
	int i = 0;
	char *p = strtok(src, delimeter);
	while (p != NULL)
	{
		dest[i++] = p;
		p = strtok(NULL, delimeter);
	}
}