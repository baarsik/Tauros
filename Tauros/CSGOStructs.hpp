#pragma once

#include <Windows.h>
#include "SourceEngine/SDK.hpp"
#include "NetvarManager.hpp"
#include "XorStr.hpp"

class CCSWeaponData
{
public:
	virtual ~CCSWeaponData() {};

	char*		consoleName;			// 0x0004
	char		pad_0008[12];			// 0x0008
	int			iMaxClip1;				// 0x0014
	int			iMaxClip2;				// 0x0018
	int			iDefaultClip1;			// 0x001C
	int			iDefaultClip2;			// 0x0020
	char		pad_0024[8];			// 0x0024
	char*		szWorldModel;			// 0x002C
	char*		szViewModel;			// 0x0030
	char*		szDroppedModel;			// 0x0034
	char		pad_0038[4];			// 0x0038
	char*		N0000023E;				// 0x003C
	char		pad_0040[56];			// 0x0040
	char*		szEmptySound;			// 0x0078
	char		pad_007C[4];			// 0x007C
	char*		szBulletType;			// 0x0080
	char		pad_0084[4];			// 0x0084
	char*		szHudName;				// 0x0088
	char*		szWeaponName;			// 0x008C
	char		pad_0090[56];			// 0x0090
	int 		WeaponType;				// 0x00C8
	int			iWeaponPrice;			// 0x00CC
	int			iKillAward;				// 0x00D0
	char*		szAnimationPrefix;		// 0x00D4
	float		flCycleTime;			// 0x00D8
	float		flCycleTimeAlt;			// 0x00DC
	float		flTimeToIdle;			// 0x00E0
	float		flIdleInterval;			// 0x00E4
	bool		bFullAuto;				// 0x00E8
	char		pad_0x00E5[3];			// 0x00E9
	int			iDamage;				// 0x00EC
	float		flArmorRatio;			// 0x00F0
	int			iBullets;				// 0x00F4
	float		flPenetration;			// 0x00F8
	float		flFlinchVelocityModifierLarge;	// 0x00FC
	float		flFlinchVelocityModifierSmall;	// 0x0100
	float		flRange;				// 0x0104
	float		flRangeModifier;		// 0x0108
	float		flThrowVelocity;		// 0x010C
	char		pad_0x010C[12];			// 0x0110
	bool		bHasSilencer;			// 0x011C
	char		pad_0x0119[3];			// 0x011D
	char*		pSilencerModel;			// 0x0120
	int			iCrosshairMinDistance;	// 0x0124
	int			iCrosshairDeltaDistance;// 0x0128 - iTeam?
	float		flMaxPlayerSpeed;		// 0x012C
	float		flMaxPlayerSpeedAlt;	// 0x0130
	float		flSpread;				// 0x0134
	float		flSpreadAlt;			// 0x0138
	float		flInaccuracyCrouch;		// 0x013C
	float		flInaccuracyCrouchAlt;	// 0x0140
	float		flInaccuracyStand;		// 0x0144
	float		flInaccuracyStandAlt;	// 0x0148
	float		flInaccuracyJumpInitial;// 0x014C
	float		flInaccuracyJump;		// 0x0150
	float		flInaccuracyJumpAlt;	// 0x0154
	float		flInaccuracyLand;		// 0x0158
	float		flInaccuracyLandAlt;	// 0x015C
	float		flInaccuracyLadder;		// 0x0160
	float		flInaccuracyLadderAlt;	// 0x0164
	float		flInaccuracyFire;		// 0x0168
	float		flInaccuracyFireAlt;	// 0x016C
	float		flInaccuracyMove;		// 0x0170
	float		flInaccuracyMoveAlt;	// 0x0174
	float		flInaccuracyReload;		// 0x0178
	int			iRecoilSeed;			// 0x017C
	float		flRecoilAngle;			// 0x0180
	float		flRecoilAngleAlt;		// 0x0184
	float		flRecoilAngleVariance;	// 0x0188
	float		flRecoilAngleVarianceAlt;	// 0x018C
	float		flRecoilMagnitude;		// 0x0190
	float		flRecoilMagnitudeAlt;	// 0x0194
	float		flRecoilMagnitudeVariance;	// 0x0198
	float		flRecoilMagnitudeVarianceAlt;	// 0x019C
	float		flRecoveryTimeCrouch;	// 0x01A0
	float		flRecoveryTimeStand;	// 0x01A4
	float		flRecoveryTimeCrouchFinal;	// 0x01A8
	float		flRecoveryTimeStandFinal;	// 0x01AC
	int			iRecoveryTransitionStartBullet;// 0x01B0 
	int			iRecoveryTransitionEndBullet;	// 0x01B4
	bool		bUnzoomAfterShot;		// 0x01B8
	bool		bHideViewModelZoomed;	// 0x01B9
	char		pad_0x01B5[2];			// 0x01BA
	char		iZoomLevels[3];			// 0x01BC
	int			iZoomFOV[2];			// 0x01C0
	float		fZoomTime[3];			// 0x01C4
	char*		szWeaponClass;			// 0x01D4
	float		flAddonScale;			// 0x01D8
	char		pad_0x01DC[4];			// 0x01DC
	char*		szEjectBrassEffect;		// 0x01E0
	char*		szTracerEffect;			// 0x01E4
	int			iTracerFrequency;		// 0x01E8
	int			iTracerFrequencyAlt;	// 0x01EC
	char*		szMuzzleFlashEffect_1stPerson; // 0x01F0
	char		pad_0x01F4[4];			 // 0x01F4
	char*		szMuzzleFlashEffect_3rdPerson; // 0x01F8
	char		pad_0x01FC[4];			// 0x01FC
	char*		szMuzzleSmokeEffect;	// 0x0200
	float		flHeatPerShot;			// 0x0204
	char*		szZoomInSound;			// 0x0208
	char*		szZoomOutSound;			// 0x020C
	float		flInaccuracyPitchShift;	// 0x0210
	float		flInaccuracySoundThreshold;	// 0x0214
	float		flBotAudibleRange;		// 0x0218
	char		pad_0x0218[8];			// 0x0220
	char*		pWrongTeamMsg;			// 0x0224
	bool		bHasBurstMode;			// 0x0228
	char		pad_0x0225[3];			// 0x0229
	bool		bIsRevolver;			// 0x022C
	bool		bCannotShootUnderwater;	// 0x0230
};

class C_CSPlayer;

class C_BaseCombatWeapon : public se::IClientEntity
{
    template<class T>
    inline T GetFieldValue(int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<DWORD>(this) + offset);
    }

	template<class T>
	T* GetFieldPointer(int offset)
	{
		return reinterpret_cast<T*>(reinterpret_cast<DWORD>(this) + offset);
	}
public:
    C_CSPlayer* GetOwner()
    {
        using namespace se;
        static int m_hOwnerEntity = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseEntity"), XorStr("m_hOwnerEntity"));
        return reinterpret_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntityFromHandle(GetFieldValue<CHandle<C_CSPlayer>>(m_hOwnerEntity)));
    }
    float NextPrimaryAttack()
    {
        static int m_flNextPrimaryAttack = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseCombatWeapon"), XorStr("LocalActiveWeaponData"), XorStr("m_flNextPrimaryAttack"));
        return GetFieldValue<float>(m_flNextPrimaryAttack);
    }
	int GetClip() {
		static int m_iClip = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseCombatWeapon"), XorStr("m_iClip1"));
		return GetFieldValue<int>(m_iClip);
	}
	const char* GetName()
	{
		using GetName_t = const char*(__thiscall*)(void*);
		return se::CallVFunction<GetName_t>(this, 378)(this);
	}
	float C_BaseCombatWeapon::GetSpread()
	{
		using GetSpread_t = float(__thiscall*)(void*);
		return se::CallVFunction<GetSpread_t>(this, 439)(this);
	}
	CCSWeaponData* GetWeaponData()
	{
		if (!this) return nullptr;
		using CCSWeaponInfo_t = CCSWeaponData*(__thiscall*)(void*);
		return se::CallVFunction<CCSWeaponInfo_t>(this, 446)(this);
	}
	float C_BaseCombatWeapon::GetInaccuracy()
	{
		using GetInaccuracy_t = float(__thiscall*)(void*);
		return se::CallVFunction<GetInaccuracy_t>(this, 469)(this);
	}
	void C_BaseCombatWeapon::UpdateAccuracyPenalty()
	{
		using UpdateAccuracyPenalty_t = void(__thiscall*)(void*);
		se::CallVFunction<UpdateAccuracyPenalty_t>(this, 470)(this);
	}

	std::string GetReadableName()
    {
		using namespace se;
	    switch (*ItemDefinitionIndex())
	    {
		case WEAPON_DEAGLE: return XorStr("Deagle");
		case WEAPON_ELITE: return XorStr("Dual Berettas");
		case WEAPON_FIVESEVEN: return XorStr("Five-Seven");
		case WEAPON_GLOCK: return XorStr("Glock-18");
		case WEAPON_AK47: return XorStr("AK-47");
		case WEAPON_AUG: return XorStr("AUG");
		case WEAPON_AWP: return XorStr("AWP");
		case WEAPON_FAMAS: return XorStr("FAMAS");
		case WEAPON_G3SG1: return XorStr("G3SG1");
		case WEAPON_GALILAR: return XorStr("Galil AR");
		case WEAPON_M249: return XorStr("M249");
		case WEAPON_M4A1: return XorStr("M4A4");
		case WEAPON_MAC10: return XorStr("MAC-10");
		case WEAPON_P90: return XorStr("P90");
		case WEAPON_UMP45: return XorStr("UMP-45");
		case WEAPON_XM1014: return XorStr("XM1014");
		case WEAPON_BIZON: return XorStr("PP-Bizon");
		case WEAPON_MAG7: return XorStr("MAG-7");
		case WEAPON_NEGEV: return XorStr("Negev");
		case WEAPON_SAWEDOFF: return XorStr("Sawed-Off");
		case WEAPON_TEC9: return XorStr("Tec-9");
		case WEAPON_TASER: return XorStr("Zeus x27");
		case WEAPON_HKP2000: return XorStr("P2000");
		case WEAPON_MP7: return XorStr("MP7");
		case WEAPON_MP9: return XorStr("MP9");
		case WEAPON_NOVA: return XorStr("Nova");
		case WEAPON_P250: return XorStr("P250");
		case WEAPON_SCAR20: return XorStr("SCAR-20");
		case WEAPON_SG556: return XorStr("SG 553");
		case WEAPON_SSG08: return XorStr("SSG 08");
		case WEAPON_FLASHBANG: return XorStr("Flashbang");
		case WEAPON_HEGRENADE: return XorStr("HE Grenade");
		case WEAPON_SMOKEGRENADE: return XorStr("Smoke Grenade");
		case WEAPON_MOLOTOV: return XorStr("Molotov");
		case WEAPON_DECOY: return XorStr("Decoy Grenade");
		case WEAPON_INCGRENADE: return XorStr("Incendiary Grenade");
		case WEAPON_C4: return XorStr("C4");
		case WEAPON_M4A1_SILENCER: return XorStr("M4A1-S");
		case WEAPON_USP_SILENCER: return XorStr("USP-S");
		case WEAPON_CZ75A: return XorStr("CZ75-Auto");
		case WEAPON_REVOLVER: return XorStr("R8 Revolver");
		case WEAPON_KNIFE: 
		case WEAPON_KNIFE_T:
		case WEAPON_KNIFE_BAYONET:
		case WEAPON_KNIFE_FLIP:
		case WEAPON_KNIFE_GUT:
		case WEAPON_KNIFE_KARAMBIT:
		case WEAPON_KNIFE_M9_BAYONET:
		case WEAPON_KNIFE_TACTICAL:
		case WEAPON_KNIFE_FALCHION:
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
		case WEAPON_KNIFE_BUTTERFLY:
		case WEAPON_KNIFE_PUSH:
			return XorStr("Knife");
		default: return XorStr("Unknown");
	    }
    }
	
	// For skinchanger purpose
	int* FallbackPaintKit()
	{
		static int m_nFallbackPaintKit = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_nFallbackPaintKit"));
		return GetFieldPointer<int>(m_nFallbackPaintKit);
	}
	int* FallbackSeed()
	{
		static int m_nFallbackSeed = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_nFallbackSeed"));
		return GetFieldPointer<int>(m_nFallbackSeed);
	}
	float* FallbackWear()
	{
		static int m_flFallbackWear = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_flFallbackWear"));
		return GetFieldPointer<float>(m_flFallbackWear);
	}
	int* FallbackStatTrak()
	{
		static int m_nFallbackStatTrak = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_nFallbackStatTrak"));
		return GetFieldPointer<int>(m_nFallbackStatTrak);
	}
	int* ItemDefinitionIndex()
    {
	    static int m_iItemDefinitionIndex = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_iItemDefinitionIndex"));
		return GetFieldPointer<int>(m_iItemDefinitionIndex);
    }
	int* EntityQuality()
	{
		static int m_iEntityQuality = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_iEntityQuality"));
		return GetFieldPointer<int>(m_iEntityQuality);
	}
	int* EntityLevel()
	{
		static int m_iEntityLevel = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_iEntityLevel"));
		return GetFieldPointer<int>(m_iEntityLevel);
	}
	int* ItemIDHigh()
	{
		static int m_iItemIDHigh = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_iItemIDHigh"));
		return GetFieldPointer<int>(m_iItemIDHigh);
	}
	int* ItemIDLow()
	{
		static int m_iItemIDLow = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_iItemIDLow"));
		return GetFieldPointer<int>(m_iItemIDLow);
	}
	int* AccountID()
	{
		static int m_iAccountID = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_iAccountID"));
		return GetFieldPointer<int>(m_iAccountID);
	}
	char* CustomName() {
		static int m_szCustomName = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_szCustomName"));
		return GetFieldPointer<char>(m_szCustomName);
	}

	// Weapon type checks
	bool IsPistol()
    {
		using namespace se;
		auto index = *ItemDefinitionIndex();
		return index == WEAPON_DEAGLE || index == WEAPON_ELITE || index == WEAPON_FIVESEVEN || index == WEAPON_GLOCK ||
			index == WEAPON_TEC9 || index == WEAPON_HKP2000 || index == WEAPON_P250 || index == WEAPON_USP_SILENCER ||
			index == WEAPON_CZ75A || index == WEAPON_REVOLVER;
    }
	bool IsGrenade()
    {
		auto classId = this->GetClientClass()->m_ClassID;
		return classId == se::EClassIds::CBaseCSGrenade || classId == se::EClassIds::CBaseGrenade ||
			classId == se::EClassIds::CHEGrenade || classId == se::EClassIds::CDecoyGrenade ||
			classId == se::EClassIds::CIncendiaryGrenade || classId == se::EClassIds::CMolotovGrenade ||
			classId == se::EClassIds::CSensorGrenade || classId == se::EClassIds::CSmokeGrenade ||
			classId == se::EClassIds::CFlashbang;
    }
	bool IsKnife()
    {
		auto classId = this->GetClientClass()->m_ClassID;
		return classId == se::EClassIds::CKnife || classId == se::EClassIds::CKnifeGG;
    }
	bool IsC4()
    {
		auto classId = this->GetClientClass()->m_ClassID;
		return classId == se::EClassIds::CC4;
    }
};


class C_CSPlayer : public se::IClientEntity
{
    template<class T>
    inline T GetFieldValue(int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<DWORD>(this) + offset);
    }

    template<class T>
    T* GetFieldPointer(int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<DWORD>(this) + offset);
    }
public:
    static C_CSPlayer* GetLocalPlayer()
    {
        return static_cast<C_CSPlayer*>(se::Interfaces::EntityList()->GetClientEntity(se::Interfaces::Engine()->GetLocalPlayer()));
    }
    C_BaseCombatWeapon* GetActiveWeapon()
    {
        using namespace se;
        static int m_hActiveWeapon = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseCombatCharacter"), XorStr("m_hActiveWeapon"));
        return static_cast<C_BaseCombatWeapon*>(Interfaces::EntityList()->GetClientEntityFromHandle(GetFieldValue<CHandle<IClientEntity>>(m_hActiveWeapon)));
    }
	float GetNextAttack()
	{
		using namespace se;
		static int m_flNextAttack = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseCombatCharacter"), XorStr("bcc_localdata"), XorStr("m_flNextAttack"));
		return GetFieldValue<float>(m_flNextAttack);
	}
    int GetHealth()
    {
        static int m_iHealth = NetvarManager::Instance()->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_iHealth"));
        return GetFieldValue<int>(m_iHealth);
    }
	int GetArmor()
	{
		static int m_ArmorValue = NetvarManager::Instance()->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_ArmorValue"));
		return GetFieldValue<int>(m_ArmorValue);
	}
	bool IsObservingTarget(C_CSPlayer* pTarget)
    {
		if (this->IsAlive())
			return false;
		
		static int m_hObserverTarget = NetvarManager::Instance()->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_hObserverTarget"));
		auto observeTarget = static_cast<C_CSPlayer*>(se::Interfaces::EntityList()->GetClientEntityFromHandle(m_hObserverTarget));
		return observeTarget != nullptr && observeTarget == pTarget;
    }
	float* FlashMaxAlpha()
    {
		static int m_flFlashMaxAlpha = NetvarManager::Instance()->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flFlashMaxAlpha"));
		return GetFieldPointer<float>(m_flFlashMaxAlpha);
    }
	bool IsScoped()
	{
		static int m_bIsScoped = NetvarManager::Instance()->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_bIsScoped"));
		return GetFieldValue<bool>(m_bIsScoped);
	}
	bool IsImmune()
	{
		static int m_bGunGameImmunity = NetvarManager::Instance()->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_bGunGameImmunity"));
		return GetFieldValue<bool>(m_bGunGameImmunity);
	}
    bool IsAlive()
    {
        static int m_lifeState = NetvarManager::Instance()->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_lifeState"));
        return GetFieldValue<int>(m_lifeState) == 0;
    }
    int GetTeamNum()
    {
        static int m_iTeamNum = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseEntity"), XorStr("m_iTeamNum"));
        return GetFieldValue<int>(m_iTeamNum);
    }
    int GetFlags()
    {
        static int m_fFlags = NetvarManager::Instance()->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_fFlags"));
        return GetFieldValue<int>(m_fFlags);
    }
    se::Vector GetViewOffset()
    {
        static int m_vecViewOffset = NetvarManager::Instance()->GetOffset(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_vecViewOffset[0]"));
        return GetFieldValue<se::Vector>(m_vecViewOffset);
    }
    se::Vector GetOrigin()
    {
        static int m_vecOrigin = NetvarManager::Instance()->GetOffset(XorStr("DT_BaseEntity"), XorStr("m_vecOrigin"));
        return GetFieldValue<se::Vector>(m_vecOrigin);
    }
    se::Vector GetEyePos()
    {
        return GetOrigin() + GetViewOffset();
    }
    se::Vector* ViewPunch()
    {
        static int m_viewPunchAngle = NetvarManager::Instance()->GetOffset(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_viewPunchAngle"));
        return GetFieldPointer<se::Vector>(m_viewPunchAngle);
    }
    se::Vector* AimPunch()
    {
        static int m_aimPunchAngle = NetvarManager::Instance()->GetOffset(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_aimPunchAngle"));
        return GetFieldPointer<se::Vector>(m_aimPunchAngle);
    }
	int GetShotsFired()
	{
		static int m_iShotsFired = NetvarManager::Instance()->GetOffset(XorStr("DT_CSPlayer"), XorStr("cslocaldata"), XorStr("m_iShotsFired"));
		return GetFieldValue<int>(m_iShotsFired);
	}
	int GetTickBase() {
		static int m_nTickBase = NetvarManager::Instance()->GetOffset(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_nTickBase"));
		return GetFieldValue<int>(m_nTickBase);
	}
};