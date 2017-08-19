#pragma once

Vector RCS_aimPunchOld;
Vector RCS_viewPunchOld;

class RCS
{
public:
	static bool CreateMove_Post(C_CSPlayer* pLocal, CUserCmd* pCmd)
	{
		if (!IsValid(pLocal))
			return true;
		
		auto punchAngles = *pLocal->AimPunch() * 2.0f;
		if (punchAngles.x != 0.0f || punchAngles.y != 0)
		{
			pCmd->viewangles -= punchAngles;
			Utils::Clamp(pCmd->viewangles);
			return false;
		}
		return true;
	}
	static void OverrideView_Pre(CViewSetup* pViewSetup)
	{
		auto pLocal = C_CSPlayer::GetLocalPlayer();
		if (!IsValid(pLocal))
			return;

		pViewSetup->angles -= *pLocal->AimPunch() * 2.0f * 0.45f;
		Utils::Clamp(pViewSetup->angles);
	}
	static void FrameStageNotify_Pre(ClientFrameStage_t stage)
	{
		auto pLocal = C_CSPlayer::GetLocalPlayer();
		if (!IsValid(pLocal))
			return;

		auto aimPunch = pLocal->AimPunch();
		auto viewPunch = pLocal->ViewPunch();
		RCS_aimPunchOld = Vector(0, 0, 0);
		RCS_viewPunchOld = Vector(0, 0, 0);
		if (aimPunch && viewPunch)
		{
			RCS_aimPunchOld = *aimPunch;
			RCS_viewPunchOld = *viewPunch;
			*aimPunch = Vector(0, 0, 0);
			*viewPunch = Vector(0, 0, 0);
		}
	}
	static void FrameStageNotify_Post(ClientFrameStage_t stage)
	{
		auto pLocal = C_CSPlayer::GetLocalPlayer();
		if (!IsValid(pLocal))
			return;

		auto aimPunch = pLocal->AimPunch();
		auto viewPunch = pLocal->ViewPunch();
		if (aimPunch && viewPunch)
		{
			*aimPunch = RCS_aimPunchOld;
			*viewPunch = RCS_viewPunchOld;
		}
	}
private:
	static bool IsValid(C_CSPlayer* pLocal)
	{
		return Options::g_bRCSEnabled && pLocal != nullptr && pLocal->IsAlive();
	}
};