#pragma once

class Trigger
{
public:
	static void CreateMove_Post(C_CSPlayer* pLocal, se::CUserCmd* pCmd)
	{
		if (!IsEnabled(pLocal))
			return;

		using namespace se;
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

		//auto vTraceEndPos = trace.endpos;
		if (!trace.m_pEnt)
			return;

		auto target = static_cast<C_CSPlayer*>(trace.m_pEnt);
		if (!target->IsAlive() || target->GetHealth() < 1 || target->IsImmune())
			return;

		if (target && target->GetTeamNum() == pLocal->GetTeamNum() && !Options::g_bTriggerFriendlyFire)
			return;

		
		
		pCmd->buttons |= IN_ATTACK;
		if (Options::g_bTriggerAutoPistol)
			AutoPistol(pLocal, pCmd);
	}
private:
	static void AutoPistol(C_CSPlayer* pLocal, se::CUserCmd* pCmd)
	{
		using namespace se;
		auto pWeapon = pLocal->GetActiveWeapon();
		if (!pWeapon)
			return;

		/*auto flServerTime = pLocal->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick;
		if (pWeapon->NextPrimaryAttack() - flServerTime > 0)
			return;*/

		static auto skipAttack = false;
		skipAttack = !skipAttack;
		if (skipAttack)
			return;

		pCmd->buttons &= *pWeapon->ItemDefinitionIndex() == WEAPON_REVOLVER ? ~IN_ATTACK2 : ~IN_ATTACK;
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