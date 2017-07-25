#pragma once

using IsReady_t = void(__cdecl*)();

class AutoAccept
{
public:
	static void PlaySound_Post(const char* szFileName)
	{
		using namespace se;
		if (!Options::g_bAutoAcceptEnabled || Interfaces::Engine()->IsInGame())
			return;

		if (strcmp(szFileName, "!UI/competitive_accept_beep.wav"))
			return;

		static auto IsReady = reinterpret_cast<IsReady_t>(Utils::FindSignature(XorStr("client.dll"), XorStr("81 66 70 00 00 F0 FF C7 46 6C 00 00 00 00 C7 46 74 00 00 00 00 5E E9 ?? ?? ?? ?? CC CC CC CC CC CC CC CC CC CC CC CC CC 55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ?? ?? ?? ?? 57")) + 0x28);
		IsReady();
		Utils::FlashTaskbarIcon();
	}
};