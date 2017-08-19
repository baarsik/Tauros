#pragma once

class RankRevealer
{
public:
	static void CreateMove_Post(C_CSPlayer* pLocal, CUserCmd* pCmd)
	{
		if (!Options::g_bRankRevealerEnabled || !(pCmd->buttons & IN_SCORE))
			return;

		SignatureHelper::ServerRankRevealAll();
	}
};