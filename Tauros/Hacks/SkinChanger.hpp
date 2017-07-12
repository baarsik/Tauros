#pragma once
#include "Options.hpp"

struct EconomyItemCfg {
	int nFallbackPaintKit = 0;
	int nFallbackSeed = 0;
	int nFallbackStatTrak = -1;
	int iEntityQuality = 4;
	char* szCustomName = nullptr;
	float flFallbackWear = 0.00001f;
};

std::unordered_map<int, EconomyItemCfg> g_SkinChangerCfg;

class SkinChanger
{
public:
	static void FrameStageNotify_Pre(se::ClientFrameStage_t stage)
	{
		using namespace se;
		if (!IsEnabled())
			return;

		InitSkins();
		if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			return;
		
		auto pLocal = C_CSPlayer::GetLocalPlayer();
		if (!pLocal || !pLocal->IsAlive())
			return;

		auto pWeapon = pLocal->GetActiveWeapon();
		if (!pWeapon)
			return;

		ApplySkin(pWeapon);
	}

	static bool ApplySkin(C_BaseCombatWeapon* pWeapon) {
		using namespace se;
		if (!pWeapon)
			return false;

		auto nWeaponIndex = *pWeapon->ItemDefinitionIndex();
		if (g_SkinChangerCfg.find(nWeaponIndex) == g_SkinChangerCfg.end())
			return false;

		// Apply our changes to the fallback variables.
		*pWeapon->FallbackPaintKit() = g_SkinChangerCfg[nWeaponIndex].nFallbackPaintKit;
		*pWeapon->EntityQuality() = g_SkinChangerCfg[nWeaponIndex].iEntityQuality;
		*pWeapon->FallbackSeed() = g_SkinChangerCfg[nWeaponIndex].nFallbackSeed;
		*pWeapon->FallbackStatTrak() = g_SkinChangerCfg[nWeaponIndex].nFallbackStatTrak;
		*pWeapon->FallbackWear() = g_SkinChangerCfg[nWeaponIndex].flFallbackWear;

		if (g_SkinChangerCfg[nWeaponIndex].szCustomName)
			sprintf_s(pWeapon->CustomName(), 32, "%s", g_SkinChangerCfg[nWeaponIndex].szCustomName);

		// Edit "m_iItemIDHigh" so fallback values will be used.
		*pWeapon->ItemIDHigh() = -1;
		return true;
	}

	static void InitSkins()
	{
		using namespace se;

		g_SkinChangerCfg[WEAPON_AWP].nFallbackPaintKit = 344;           // Dragon Lore
		g_SkinChangerCfg[WEAPON_AK47].nFallbackPaintKit = 302;          // Vulkan
		g_SkinChangerCfg[WEAPON_M4A1].nFallbackPaintKit = 309;          // Howl
		g_SkinChangerCfg[WEAPON_M4A1_SILENCER].nFallbackPaintKit = 587; // Mecha Industries (M4A1-S)
		g_SkinChangerCfg[WEAPON_SSG08].nFallbackPaintKit = 222;         // Blood in the Water
		g_SkinChangerCfg[WEAPON_P90].nFallbackPaintKit = 359;           // Asiimov (P90)
		g_SkinChangerCfg[WEAPON_GLOCK].nFallbackPaintKit = 38;          // Fade (Glock-18)
		g_SkinChangerCfg[WEAPON_USP_SILENCER].nFallbackPaintKit = 313;  // Orion
		g_SkinChangerCfg[WEAPON_P250].nFallbackPaintKit = 551;          // Asiimov (P250)
		g_SkinChangerCfg[WEAPON_SCAR20].nFallbackPaintKit = 312;        // Cyrex
		g_SkinChangerCfg[WEAPON_MAG7].nFallbackPaintKit = 39;           // Bulldozer
		g_SkinChangerCfg[WEAPON_MP9].nFallbackPaintKit = 39;            // Bulldozer
		g_SkinChangerCfg[WEAPON_G3SG1].nFallbackPaintKit = 493;         // Bulldozer
		g_SkinChangerCfg[WEAPON_MP7].nFallbackPaintKit = 102;           // Whiteout
		g_SkinChangerCfg[WEAPON_AUG].nFallbackPaintKit = 455;           // Akihabara Accept
		g_SkinChangerCfg[WEAPON_FAMAS].nFallbackPaintKit = 626;         // Mecha Industries (FAMAS)
		g_SkinChangerCfg[WEAPON_NOVA].nFallbackPaintKit = 62;           // Bloomstick
		g_SkinChangerCfg[WEAPON_DEAGLE].nFallbackPaintKit = 351;        // Conspiracy
		g_SkinChangerCfg[WEAPON_REVOLVER].nFallbackPaintKit = 522;      // Fade (Revolver)
	}
private:
	static bool IsEnabled()
	{
		if (!Options::g_bSkinChangerEnabled)
			return false;
		
		return true;
	}
};