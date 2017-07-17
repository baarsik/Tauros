#pragma once

class Bhop
{
public:
	static void CreateMove_Post(C_CSPlayer* pLocal, se::CUserCmd* pCmd)
	{
		if (Options::g_bBHopEnabled && pLocal->IsAlive())
			return;
		
		if (pCmd->buttons & IN_JUMP && !(pLocal->GetFlags() & static_cast<int>(se::EntityFlags::FL_ONGROUND)))
		{
			pCmd->buttons &= ~IN_JUMP;
		}

		if (Options::g_bAutoStrafeEnabled && !(pLocal->GetFlags() & static_cast<int>(se::EntityFlags::FL_ONGROUND)))
		{
			if (pCmd->mousedx < 0)
			{
				pCmd->sidemove = -450.0f;
			}
			else if (pCmd->mousedx > 0)
			{
				pCmd->sidemove = 450.0f;
			}
		}
	}
};