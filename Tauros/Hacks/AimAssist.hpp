#pragma once

bool g_bIsAttacking = false;
C_CSPlayer* g_pAimLockTarget = nullptr;
class AimAssist
{
public:
	static void CreateMove_Post(C_CSPlayer* pLocal, CUserCmd* pCmd)
	{
		g_bIsAttacking = pCmd->buttons & IN_ATTACK;
	}
	static void OverrideMouseInput_Post(float* x, float* y)
	{
		auto pLocal = C_CSPlayer::GetLocalPlayer();
		if (!IsEnabled(pLocal))
		{
			g_pAimLockTarget = nullptr;
			return;
		}

		if (!g_bIsAttacking && !Options::g_bAimAssistAutoShoot && !IsTriggerEnabled())
		{
			g_pAimLockTarget = nullptr;
			return;
		}

		auto pWeapon = pLocal->GetActiveWeapon();
		if (!pWeapon || pWeapon->IsKnife() || pWeapon->IsGrenade() || pWeapon->IsC4())
		{
			g_pAimLockTarget = nullptr;
			return;
		}

		if (g_pAimLockTarget != nullptr && !g_pAimLockTarget->IsAlive())
			g_pAimLockTarget = nullptr;

		auto bone = HEAD_0;
		Vector qDelta;
		while (true)
		{
			auto pTarget = g_pAimLockTarget == nullptr ? GetClosestPlayer(pLocal, Options::g_fAimAssistFov, bone) : g_pAimLockTarget;
			if (pTarget && TraceBone(pLocal, pTarget, bone))
			{
				qDelta = GetDelta(pLocal, pTarget, bone);
				if (Options::g_bAimAssistLockTarget)
					g_pAimLockTarget = pTarget;

				break;
			}

			if (bone == HEAD_0) bone = NECK_0;
			else if (bone == NECK_0) bone = SPINE_3;
			else if (bone == SPINE_3) bone = SPINE_2;
			else if (bone == SPINE_2) bone = SPINE_1;
			else if (bone == SPINE_1) bone = SPINE_0;
			else
			{
				g_pAimLockTarget = nullptr;
				return;
			}
		}

		// Adjust cursor position (0.022f = no smoothing)
		auto xSmooth = Options::g_fAimAssistSmooth;
		auto ySmooth = Options::g_fAimAssistSmooth / Options::g_fAimAssistVerticalSmoothMultiplier;
		qDelta.x /= xSmooth >= 0.022f ? xSmooth : 0.022f;
		qDelta.y /= ySmooth >= 0.022f ? ySmooth : 0.022f;

		auto vMouse = Options::g_bAimAssistLockMouse ? Vector(0, 0, 0) : Vector(*static_cast<float*>(x), *static_cast<float*>(y), 0.0f);
		auto vDelta = Vector(qDelta.y, -qDelta.x, 0.0f);
		vDelta.x = (vDelta.x + vMouse.x) / 2.0f;
		vDelta.y = (vDelta.y + vMouse.y) / 2.0f;
		*static_cast<float*>(x) = vDelta.x;
		*static_cast<float*>(y) = vDelta.y;
	}

	static bool TraceBone(C_CSPlayer* pLocal, C_CSPlayer* pTarget, ECSPlayerBones bone)
	{
		if (Options::g_bAimAssistIgnoreObstacles)
			return true;
		
		trace_t trace;
		Ray_t ray;
		CTraceFilter filter;
		auto vTraceForward = Vector(0, 0, 0);
		auto vEyePos = pLocal->GetEyePos();
		auto vAimBone = Utils::GetEntityBone(pTarget, bone);
		auto qAimAngles = Utils::CalcAngle(vEyePos, vAimBone);
		Utils::AngleVectors(qAimAngles, &vTraceForward);
		ray.Init(vEyePos, vEyePos + vTraceForward * 8192.0f);
		filter.pSkip = pLocal;
		Interfaces::EngineTrace()->TraceRay(ray, (MASK_SHOT_HULL | CONTENTS_HITBOX), &filter, &trace);
		if (trace.fraction == 1.f || !trace.m_pEnt)
			return false;

		auto vTraceEndPos = trace.endpos;
		if (!Options::g_bAimAssistIgnoreSmoke && SignatureHelper::LineThroughSmoke(vEyePos, vTraceEndPos))
			return false;

		auto target = static_cast<C_CSPlayer*>(trace.m_pEnt);
		if (!target || !target->IsAlive() || target->GetHealth() < 1 || target->IsImmune() || target->IsDormant())
			return false;

		if (target->GetTeamNum() == pLocal->GetTeamNum() && !Options::g_bAimAssistFriendlyFire)
			return false;

		return true;
	}

	static Vector GetDelta(C_CSPlayer* pLocal, C_CSPlayer* pTarget, ECSPlayerBones bone)
	{
		QAngle qViewAngles;
		Interfaces::Engine()->GetViewAngles(qViewAngles);
		if (!Options::g_bRCSEnabled)
			qViewAngles += *pLocal->AimPunch() * 2.0f;
		Utils::Clamp(qViewAngles);
		auto vEyePos = pLocal->GetEyePos();
		auto vAimBone = Utils::GetEntityBone(pTarget, bone);
		auto qAimAngles = Utils::CalcAngle(vEyePos, vAimBone);
		auto qDelta = qViewAngles - qAimAngles;
		Utils::Clamp(qDelta);
		return qDelta;
	}

	static C_CSPlayer* GetClosestPlayer(C_CSPlayer* pLocal, float fov, ECSPlayerBones bone)
	{
		C_CSPlayer* pTarget = nullptr;
		auto maxFov = fov;

		QAngle viewAngles;
		Interfaces::Engine()->GetViewAngles(viewAngles);
		if (!Options::g_bRCSEnabled)
			viewAngles += *pLocal->AimPunch() * 2.0f;
		auto vEyePos = pLocal->GetEyePos();

		for (auto i = 1; i <= Interfaces::Engine()->GetMaxClients(); i++)
		{
			auto pPotentialTarget = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(i));
			if (!pPotentialTarget || pPotentialTarget == pLocal)
				continue;

			if (!pPotentialTarget->IsAlive() || pPotentialTarget->IsDormant() || pPotentialTarget->IsImmune())
				continue;

			if (pPotentialTarget->GetTeamNum() == pLocal->GetTeamNum() && !Options::g_bAimAssistFriendlyFire)
				continue;

			auto vTargetBone = Utils::GetEntityBone(pPotentialTarget, bone);
			auto fTargetFov = GetFov(viewAngles, Utils::CalcAngle(vEyePos, vTargetBone));
			if (fTargetFov > maxFov)
				continue;

			pTarget = pPotentialTarget;
			maxFov = fTargetFov;
		}
		return pTarget;
	}
private:
	static bool IsEnabled(C_CSPlayer* pLocal)
	{
		return pLocal && Options::g_bAimAssistEnabled && pLocal->IsAlive();
	}

	static bool IsTriggerEnabled()
	{
		return Options::g_bTriggerEnabled && Options::g_bTriggerAimSynergy && (GetAsyncKeyState(Options::KeysID[Options::g_iTriggerKey]) || Options::g_bTriggerAlwaysActive);
	}

	static float GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
	{
		auto delta = aimAngle - viewAngle;
		Utils::Clamp(delta);
		return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
	}
};