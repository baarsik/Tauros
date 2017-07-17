#pragma once

bool g_bLMBDown = false;
bool g_bRMBDown = false;
class AutoPistol
{
public:
	static void CreateMove_Post(C_CSPlayer* pLocal, se::CUserCmd* pCmd)
	{
		using namespace se;
		if (!Options::g_bAutoPistolEnabled)
			return;

		auto pWeapon = pLocal->GetActiveWeapon();
		if (!pWeapon || !pWeapon->IsPistol() || pWeapon->GetClip() == 0)
			return;

		auto isRevolver = *pWeapon->ItemDefinitionIndex() == WEAPON_REVOLVER;
		if (isRevolver && !g_bRMBDown || !isRevolver && !g_bLMBDown)
			return;

		auto curtime = pLocal->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick;
		if (pWeapon->NextPrimaryAttack() >= curtime || pLocal->GetNextAttack() >= curtime)
			pCmd->buttons &= isRevolver ? ~IN_ATTACK2 : ~IN_ATTACK;
		else
			pCmd->buttons |= isRevolver ? IN_ATTACK2 : IN_ATTACK;
	}
	static void CheckToggle(bool vecPressedKeys[256])
	{
		g_bLMBDown = vecPressedKeys[VK_LBUTTON];
		g_bRMBDown = vecPressedKeys[VK_RBUTTON];
	}
};