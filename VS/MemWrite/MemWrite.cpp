#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUF_SIZE 2051
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");
u_char szMsg[BUF_SIZE];

int _tmain()
{
	HANDLE hMapFile;
	LPCTSTR pBuf;

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		szName);                 // name of mapping object

	if (hMapFile == NULL)
	{
		int err = GetLastError();
		_tprintf(TEXT("Could not create file mapping object (%d).\n"),
			err);
		return 1;
	}
	pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return 1;
	}

	memset(szMsg, 70, BUF_SIZE);
	while (true)
	{
		CopyMemory((PVOID)pBuf, szMsg, BUF_SIZE);
		_getch();
	}
	

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}

