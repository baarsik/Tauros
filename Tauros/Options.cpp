#include "Options.hpp"

namespace Options
{
	const char* Keys[19] = {
		"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
		"F", "H", "J", "Z", "X", "C", "V"
	};
	const int KeysID[19] = {
		0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B,
		0x46, 0x48, 0x4A, 0x5A, 0x58, 0x43, 0x56
	};

    bool g_bMainWindowOpen = false;

	// AimAssist
	bool g_bAimAssistEnabled = true;
	bool g_bAimAssistFriendlyFire = false;
	bool g_bAimAssistLockTarget = true;
	bool g_bAimAssistIgnoreObstacles = false;
	bool g_bAimAssistAutoShoot = false;
	bool g_bAimAssistLockMouse = true;
	float g_fAimAssistFov = 1.55f;
	float g_fAimAssistSmooth = 0.195f;
	float g_fAimAssistVerticalSmoothMultiplier = 1.75f;

	// Visuals (1)
    bool g_bESPEnabled = true;
    int g_iESPShowBoxes = 2;
	int g_iESPShowHealth = 3;
	int g_iESPShowBones = 1;
    int g_iESPShowNames = 3;
	int g_iESPShowDistance = 0;
	int g_iESPShowWeapon = 2;
	float g_fESPAllyColor[4] = { 1.0f, 239.0f / 255, 213.0f / 255, 1.0f };
	float g_fESPEnemyColor[4] = { 1.0f, 151.0f / 255, 0.0f, 1.0f };
	// Visuals (2)
	bool g_bChamsEnabled = true;
	bool g_bChamsShowInvisible = true;
	int g_iChamsVisibleMaterialId = 1;
	float g_fChamsVisibleColor[3] = { 152.0f / 255, 245.0f / 255, 1.0f };
	int g_iChamsInvisibleMaterialId = 1;
	float g_fChamsInvisibleColor[3] = { 1.0f, 127.0f / 255, 0.0f };
	// Visuals (3)
	bool g_bCleanScreenshot = true;
	bool g_bC4TimerEnabled = true;
	bool g_bDamageInformerEnabled = true;
	bool g_bNoSmokeEnabled = false;
	bool g_bNoFlashEnabled = false;
	float g_fNoFlashAmount = 30.0f;
	const char* g_szHandsDisplay[3] = {
		"Normal",
		"No hands",
		"Wireframe"
	};
	int g_iHandsDisplay = 0;

	// Trigger
	bool g_bTriggerEnabled = true;
	int g_iTriggerKey = 12;
	bool g_bTriggerAlwaysActive = false;
	bool g_bTriggerFriendlyFire = false;
	bool g_bTriggerAimSynergy = true;

	// Skin
	bool g_bSkinChangerEnabled = false;

	// Misc
    bool g_bRCSEnabled = true;
	bool g_bAutoPistolEnabled = true;
    bool g_bBHopEnabled = true;
	bool g_bAutoStrafeEnabled = true;
	bool g_bAutoAcceptEnabled = true;
	bool g_bRankRevealerEnabled = true;

	// Configs
	const char* g_szGuiSchema[2] = {
		"Black",
		"White"
	};
	int g_iGuiSchema = 0;
}
