#pragma once

namespace Options
{
	extern const char* Keys[19];
	extern const int KeysID[19];

    extern bool g_bMainWindowOpen;

	// AimAssist
	extern bool g_bAimAssistEnabled;
	extern bool g_bAimAssistFriendlyFire;
	extern bool g_bAimAssistIgnoreObstacles;
	extern bool g_bAimAssistAutoShoot;
	extern bool g_bAimAssistLockMouse;
	extern float g_fAimAssistFov;
	extern float g_fAimAssistSmooth;
	extern float g_fAimAssistVerticalSmoothMultiplier;

	// Visuals (1)
    extern bool g_bESPEnabled;
    extern int g_iESPShowBoxes;
	extern int g_iESPShowHealth;
	extern int g_iESPShowBones;
    extern int g_iESPShowNames;
	extern int g_iESPShowDistance;
	extern int g_iESPShowWeapon;
	extern float g_fESPAllyColor[4];
	extern float g_fESPEnemyColor[4];
	// Visuals (2)
	extern bool g_bChamsEnabled;
	extern int g_iChamsVisibleMaterialId;
	extern float g_fChamsVisibleColor[3];
	extern int g_iChamsInvisibleMaterialId;
	extern float g_fChamsInvisibleColor[3];
	// Visuals (3)
	extern bool g_bCleanScreenshot;
	extern bool g_bDamageIndicatorEnabled;
	extern bool g_bNoSmokeEnabled;
	extern bool g_bNoFlashEnabled;
	extern float g_fNoFlashAmount;
	extern const char* g_szHandsDisplay[3];
	extern int g_iHandsDisplay;

	// Trigger
	extern bool g_bTriggerEnabled;
	extern int g_iTriggerKey;
	extern bool g_bTriggerAlwaysActive;
	extern bool g_bTriggerFriendlyFire;

	// Skin
	extern bool g_bSkinChangerEnabled;

	// Misc
    extern bool g_bRCSEnabled;
	extern bool g_bBHopEnabled;
	extern bool g_bAutoAcceptEnabled;

	// Configs
	extern const char* g_szGuiSchema[2];
	extern int g_iGuiSchema;
}

