#pragma once

class Trigger
{
public:
	static void CreateMove_Post(C_CSPlayer* pLocal, CUserCmd* pCmd)
	{
		if (!IsEnabled(pLocal))
			return;

		auto vTraceForward = Vector(0, 0, 0);
		auto vTraceAngles = pCmd->viewangles;
		if (!Options::g_bRCSEnabled)
		{
			auto punchAngles = *pLocal->AimPunch() * 2.0f;
			if (punchAngles.x != 0.0f || punchAngles.y != 0)
			{
				vTraceAngles -= punchAngles;
				Utils::Clamp(vTraceAngles);
			}
		}
		Utils::AngleVectors(vTraceAngles, &vTraceForward);
		auto vTraceStart = pLocal->GetEyePos();
		auto vTraceEnd = vTraceStart + vTraceForward * 8192.0f;

		trace_t trace;
		Ray_t ray;
		CTraceFilter filter;
		filter.pSkip = pLocal;
		ray.Init(vTraceStart, vTraceEnd);
		Interfaces::EngineTrace()->TraceRay(ray, (MASK_SHOT_HULL | CONTENTS_HITBOX), &filter, &trace);
		if (trace.fraction == 1.f)
			return;

		if (!trace.m_pEnt)
			return;

		auto vTraceEndPos = trace.endpos;
		if (!Options::g_bTriggerIgnoreSmoke && SignatureHelper::LineThroughSmoke(vTraceStart, vTraceEndPos))
			return;

		auto target = static_cast<C_CSPlayer*>(trace.m_pEnt);
		if (!target->IsAlive() || target->GetHealth() < 1 || target->IsImmune())
			return;

		if (target && target->GetTeamNum() == pLocal->GetTeamNum() && !Options::g_bTriggerFriendlyFire)
			return;

		auto pWeapon = pLocal->GetActiveWeapon();
		if (!pWeapon || pWeapon->GetClip() == 0)
			return;

		if (pWeapon->IsPistol())
			AutoPistol(pLocal, pWeapon, pCmd);
		else
			pCmd->buttons |= IN_ATTACK;
	}
private:
	static void AutoPistol(C_CSPlayer* pLocal, C_BaseCombatWeapon* pWeapon, CUserCmd* pCmd)
	{
		// Let AutoPistol class handle this situation
		if (Options::g_bAutoPistolEnabled && (GetKeyState(VK_LBUTTON) & 0x100) != 0)
			return;

		if (pWeapon->NextPrimaryAttack() > pLocal->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick)
			return;
		
		if (*pWeapon->ItemDefinitionIndex() == WEAPON_REVOLVER)
			pCmd->buttons |= IN_ATTACK2;
		else
			pCmd->buttons |= IN_ATTACK;
	}

	static bool IsEnabled(C_CSPlayer* pLocal)
	{
		if (!Options::g_bTriggerEnabled || pLocal == nullptr || !pLocal->IsAlive())
			return false;

		if (!GetAsyncKeyState(Options::KeysID[Options::g_iTriggerKey]) && !Options::g_bTriggerAlwaysActive)
			return false;

		auto weapon = pLocal->GetActiveWeapon();
		if (!weapon)
			return false;

		return !(weapon->IsKnife() || weapon->IsGrenade() || weapon->IsC4());
	}
};