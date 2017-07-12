#include "stdafx.h"
#include "Injector.h"
#include <tlhelp32.h>

DWORD Injector::FindProcessId(const std::string& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	auto processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}

MainGUIStatus Injector::Inject(const std::string& processName, char* DLL_NAME)
{
	return Inject(FindProcessId(processName), DLL_NAME);
}

MainGUIStatus Injector::Inject(DWORD pID, char* DLL_NAME)
{
	if (!pID)
		return STATUS_GAME_NOTFOUND;

	auto dllPath = GetCurrentPath() + DLL_NAME;
	if (auto file = fopen(dllPath.c_str(), "r"))
		fclose(file);
	else
		return STATUS_BIN_NOTFOUND;

	auto Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
	if (!Proc)
		return STATUS_GAME_INJECTFAILED;

	auto LoadLibAddy = static_cast<LPVOID>(GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"));

	// Allocate space in the process for our DLL 
	auto RemoteString = static_cast<LPVOID>(VirtualAllocEx(Proc, nullptr, dllPath.size(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

	// Write the string name of our DLL in the memory allocated 
	WriteProcessMemory(Proc, static_cast<LPVOID>(RemoteString), dllPath.c_str(), dllPath.size(), nullptr);

	// Load our DLL 
	CreateRemoteThread(Proc, nullptr, NULL, static_cast<LPTHREAD_START_ROUTINE>(LoadLibAddy), static_cast<LPVOID>(RemoteString), NULL, nullptr);

	CloseHandle(Proc);
	return STATUS_GAME_INJECTED;
}

std::string Injector::GetCurrentPath()
{
	char result[MAX_PATH] = { "" };
	GetModuleFileName(nullptr, result, MAX_PATH);
	std::string::size_type pos = std::string(result).find_last_of("\\/");
	return std::string(result).substr(0, pos + 1);
}