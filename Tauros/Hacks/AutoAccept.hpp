#pragma once

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

		SignatureHelper::IsReady();
		Utils::FlashTaskbarIcon();
	}
};