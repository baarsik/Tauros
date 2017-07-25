#pragma once

class Bhop
{
public:
	static void CreateMove_Post(C_CSPlayer* pLocal, se::CUserCmd* pCmd)
	{
		DoStrafe(pLocal, pCmd);
		DoBhop(pLocal, pCmd);
	}
private:
	static void DoBhop(C_CSPlayer* pLocal, se::CUserCmd* pCmd)
	{
		if (!Options::g_bBHopEnabled || !pLocal->IsAlive())
			return;

		if (pLocal->GetFlags() & static_cast<int>(se::EntityFlags::FL_ONGROUND))
			return;

		if (pCmd->buttons & IN_JUMP)
			pCmd->buttons &= ~IN_JUMP;
	}

	static void DoStrafe(C_CSPlayer* pLocal, se::CUserCmd* pCmd)
	{
		if (!Options::g_bAutoStrafeEnabled)
			return;

		if (pCmd->mousedx <= 1 && pCmd->mousedx >= -1)
			return;

		if (pLocal->GetFlags() & static_cast<int>(se::EntityFlags::FL_ONGROUND))
			return;
		
		if (pCmd->buttons & IN_JUMP)
			pCmd->sidemove = pCmd->mousedx < 0 ? -450.f : 450.f;
	}
};