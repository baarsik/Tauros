// Launcher.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

int main()
{
	auto gui = MainGUI();
	gui.SetStatus(STATUS_BIN_UPDATED);
	while (true)
	{
		if (gui.GetGameStatus() == STATUS_GAME_NOTFOUND || gui.GetGameStatus() == STATUS_GAME_INJECTFAILED)
		{
			auto status = Injector::Inject("csgo.exe", "Tauros.dll");
			gui.SetStatus(status);
			if (status != STATUS_GAME_INJECTED)
			{
				Sleep(500);
				continue;
			}
			gui.WriteLine("Game was successfully injected", STATUS_SUCCESS);
		}
		if (gui.GetGameStatus() == STATUS_GAME_INJECTED && Injector::FindProcessId("csgo.exe") == 0)
		{
			gui.WriteLine("Game was closed");
			gui.SetStatus(STATUS_GAME_NOTFOUND);
			Sleep(500);
			continue;
		}
		Sleep(100);
	}
    return 0;
}