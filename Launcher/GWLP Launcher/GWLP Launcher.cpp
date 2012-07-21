/**
* A Launcher to connect to GWLP servers.
* Author: ACB
**/

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION    processInformation;

	ZeroMemory( &startupInfo, sizeof( STARTUPINFO ));
	startupInfo.cb = sizeof( STARTUPINFO );
	ZeroMemory( &processInformation, sizeof( PROCESS_INFORMATION ));

	//Start GW
	if(CreateProcess("Gw.exe",GetCommandLineA(),NULL,NULL,false,CREATE_SUSPENDED,NULL,NULL,&startupInfo,&processInformation) == FALSE){
		printf("Unable to start GW\n");
		system("PAUSE");
		return -1;
	}
	
	LPCSTR pcDllName = "GWLP.dll";
	LPVOID pStringInRemoteProcess;
	pStringInRemoteProcess = VirtualAllocEx( processInformation.hProcess, 0, strlen(pcDllName)+1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	WriteProcessMemory( processInformation.hProcess, pStringInRemoteProcess, pcDllName, strlen(pcDllName)+1, NULL);
	HANDLE hThread = CreateRemoteThread(processInformation.hProcess, 0, 0,(LPTHREAD_START_ROUTINE)GetProcAddress( GetModuleHandleA("kernel32.dll") , "LoadLibraryA"), pStringInRemoteProcess, 0, 0);
	WaitForSingleObject(hThread,INFINITE);
	VirtualFreeEx(processInformation.hProcess,pStringInRemoteProcess,strlen(pcDllName) + 1,MEM_FREE);
	CloseHandle(hThread);
	ResumeThread(processInformation.hThread);

	return 0;
}

