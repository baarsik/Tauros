#pragma once
class Injector
{
public:
	static MainGUIStatus Inject(const std::string& processName, char* DLL_NAME);
	static MainGUIStatus Inject(DWORD pID, char* DLL_NAME);
	static DWORD FindProcessId(const std::string& processName);
	static std::string GetCurrentPath();
};

