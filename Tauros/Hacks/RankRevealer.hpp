#pragma once

class RankRevealer
{
public:
	static void CreateMove_Post(C_CSPlayer* pLocal, se::CUserCmd* pCmd)
	{
		using namespace se;
		if (!Options::g_bRankRevealerEnabled || !(pCmd->buttons & IN_SCORE))
			return;

		SignatureHelper::ServerRankRevealAll();
	}
};