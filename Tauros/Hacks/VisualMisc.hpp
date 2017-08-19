#pragma once

class NoFlash
{
public:
	static void FrameStageNotify_Pre(ClientFrameStage_t stage)
	{
		if (!Interfaces::Engine()->IsInGame())
			return;

		if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			return;

		auto pLocal = C_CSPlayer::GetLocalPlayer();
		if (!pLocal)
			return;

		if (Options::g_bNoFlashEnabled)
			*pLocal->FlashMaxAlpha() = Options::g_fNoFlashAmount * 2.55f;
		else
			*pLocal->FlashMaxAlpha() = 255.0f;
	}
};

std::vector<const char*> smoke_materials = {
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust"
};

class NoSmoke
{
public:
	static void FrameStageNotify_Pre(ClientFrameStage_t stage)
	{
		if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END)
			return;

		for (auto material_name : smoke_materials)
		{
			auto mat = Interfaces::MaterialSystem()->FindMaterial(material_name, XorStr(TEXTURE_GROUP_OTHER));
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Options::g_bNoSmokeEnabled);
		}
	}
};

class Hands
{
public:
	static void DrawModelExecute_Pre(void* ecx, IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
	{
		if (Options::g_iHandsDisplay == 0 || !pInfo.pModel)
			return;
		
		if (strstr(pInfo.pModel->name, XorStr("arms")) == nullptr)
			return;

		auto hands = Interfaces::MaterialSystem()->FindMaterial(pInfo.pModel->name, XorStr(TEXTURE_GROUP_MODEL));
		hands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Options::g_iHandsDisplay == 1);
		hands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Options::g_iHandsDisplay == 2);
		Interfaces::ModelRender()->ForcedMaterialOverride(hands);
	}
};
