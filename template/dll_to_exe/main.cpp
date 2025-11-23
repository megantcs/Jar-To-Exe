#define _WIN64

#include "injector.h"
#include "header.h"
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, wchar_t* argv[], wchar_t* envp[]) {

	wchar_t* dllPath;
	DWORD PID = 0;
	std::string buf;

	std::cin >> buf;

	try {
		PID = stoi(buf);
	}
	catch (...) {}

	printf("Process pid: %d\n", PID);

	TOKEN_PRIVILEGES priv = { 0 };
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		priv.PrivilegeCount = 1;
		priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
			AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

		CloseHandle(hToken);
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProc) return -2;
	
	auto FileSize = __SIZE__;
	BYTE* pSrcData = rawData;

	if (!ManualMapDll(hProc, pSrcData, FileSize)) {
		delete[] pSrcData;
		CloseHandle(hProc);
	}
	delete[] pSrcData;

	CloseHandle(hProc);

	return 0;
}
