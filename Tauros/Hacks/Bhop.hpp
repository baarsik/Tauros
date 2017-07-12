#pragma once

class Bhop
{
public:
	static void CreateMove_Post(C_CSPlayer* pLocal, se::CUserCmd* pCmd)
	{
		if (!IsEnabled(pLocal))
			return;
		
		if ((pCmd->buttons & IN_JUMP) && !(pLocal->GetFlags() & static_cast<int>(se::EntityFlags::FL_ONGROUND)))
			pCmd->buttons &= ~IN_JUMP;
	}
private:
	static bool IsEnabled(C_CSPlayer* pLocal)
	{
		return Options::g_bBHopEnabled && pLocal->IsAlive();
	}
};