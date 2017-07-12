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

		if (strcmp(szFileName, "UI/competitive_accept_beep.wav"))
			return;

		static auto IsReady = reinterpret_cast<IsReady_t>(static_cast<DWORD>(Utils::FindSignature(XorStr("client.dll"), XorStr("55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ?? ?? ?? ?? 57 8B 8E"))));
		IsReady();
		Utils::FlashTaskbarIcon();
	}
};