#pragma once

class Chams
{
public:
	static void DrawModelExecute_Pre(void* ecx, se::IMatRenderContext* ctx, const se::DrawModelState_t &state, const se::ModelRenderInfo_t &pInfo, se::matrix3x4_t *pCustomBoneToWorld)
	{
		using namespace se;
		if (!Options::g_bChamsEnabled || !pInfo.pModel)
			return;

		std::string modelName = Interfaces::ModelInfo()->GetModelName(pInfo.pModel);
		if (modelName.find(XorStr("models/player")) != std::string::npos)
			ApplyPlayerChams(ecx, ctx, state, pInfo, pCustomBoneToWorld);
	}
private:
	static void ApplyPlayerChams(void* ecx, se::IMatRenderContext* ctx, const se::DrawModelState_t &state, const se::ModelRenderInfo_t &pInfo, se::matrix3x4_t *pCustomBoneToWorld)
	{
		using namespace se;

		auto pLocal = C_CSPlayer::GetLocalPlayer();
		if (!pLocal)
			return;

		auto pTarget = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(pInfo.entity_index));
		if (!pTarget || pTarget == pLocal || pTarget->IsDormant() || !pTarget->IsAlive() || pTarget->GetHealth() < 1)
			return;

		auto isImmune = pTarget->IsImmune();
		if (pTarget->GetTeamNum() == pLocal->GetTeamNum())
			return;

		if (Options::g_bChamsShowInvisible)
		{
			auto invisMaterial = MatHelper.GetMaterialById(Options::g_iChamsInvisibleMaterialId, true);
			invisMaterial->ColorModulate(Options::g_fChamsInvisibleColor[0], Options::g_fChamsInvisibleColor[1], Options::g_fChamsInvisibleColor[2]);
			invisMaterial->AlphaModulate(isImmune ? 0.4f : 1.0f);
			Interfaces::ModelRender()->ForcedMaterialOverride(invisMaterial);
			Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
		}

		auto visMaterial = MatHelper.GetMaterialById(Options::g_iChamsVisibleMaterialId, false);
		visMaterial->ColorModulate(Options::g_fChamsVisibleColor[0], Options::g_fChamsVisibleColor[1], Options::g_fChamsVisibleColor[2]);
		visMaterial->AlphaModulate(isImmune ? 0.4f : 1.0f);
		Interfaces::ModelRender()->ForcedMaterialOverride(visMaterial);
	}
};