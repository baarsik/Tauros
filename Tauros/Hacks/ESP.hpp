#pragma once
#include "CSGOStructs.hpp"
#include "Options.hpp"
#include "Utils.hpp"

struct FontSize
{
	int width;
	int height;
};

class ESP
{
public:
	static void PaintTraverse_Post()
	{
		using namespace se;
		if (!Options::g_bESPEnabled)
			return;

		for (auto i = 1; i < Interfaces::Engine()->GetMaxClients(); i++)
		{
			if (i == Interfaces::Engine()->GetLocalPlayer())
				continue;

			auto pTarget = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(i));
			auto pLocal = C_CSPlayer::GetLocalPlayer();
			if (!pTarget || !pLocal)
				continue;

			if (!pTarget->IsAlive() || pTarget->IsDormant())
				continue;

			if (pTarget->GetClientClass()->m_ClassID == EClassIds::CCSPlayer)
			{
				auto vOrigin = pTarget->GetOrigin();
				auto vHead = Utils::GetEntityBone(pTarget, ECSPlayerBones::HEAD_0);
				vHead.z += 15.0f;

				Vector vScreenOrigin, vScreenHead;
				if (!Utils::WorldToScreen(vHead, vScreenHead) || !Utils::WorldToScreen(vOrigin, vScreenOrigin))
					continue;
				
				RenderHealth(pLocal, pTarget, vScreenHead, vScreenOrigin);
				RenderBoxes(pLocal, pTarget, vScreenHead, vScreenOrigin);
				RenderBones(pLocal, pTarget);
				RenderTop(pLocal, pTarget, vScreenHead);
				RenderBottom(pLocal, pTarget, vScreenOrigin);
			}
		}
	}

private:
	static void RenderHealth(C_CSPlayer* pLocal, C_CSPlayer* pTarget, se::Vector vScreenHead, se::Vector vScreenOrigin)
	{
		using namespace se;
		if (Options::g_iESPShowHealth == 0)
			return;

		auto isEnemy = pLocal->GetTeamNum() != pTarget->GetTeamNum();
		if (Options::g_iESPShowHealth == (isEnemy ? 1 : 2))
			return;

		auto width = int(abs(vScreenHead.y - vScreenOrigin.y) * 0.65f);
		auto height = 4;
		auto x = int(vScreenHead.x - width / 2);
		auto y = int(vScreenHead.y - height * 2);

		auto healthNormalized = pTarget->GetHealth() >= 100 ? 1.0f : pTarget->GetHealth() / 100.0f;

		auto allyColor = Options::g_fESPAllyColor;
		auto enemyColor = Options::g_fESPEnemyColor;
		auto foreground = isEnemy
			? Color(int(enemyColor[0] * 255.0f), int(enemyColor[1] * 255.0f), int(enemyColor[2] * 255.0f), 255)
			: Color(int(allyColor[0] * 255.0f), int(allyColor[1] * 255.0f), int(allyColor[2] * 255.0f), 255);
		auto background = isEnemy // foreground / 2.5f
			? Color(int(enemyColor[0] * 102.0f), int(enemyColor[1] * 102.0f), int(enemyColor[2] * 102.0f), 255)
			: Color(int(allyColor[0] * 102.0f), int(allyColor[1] * 102.0f), int(allyColor[2] * 102.0f), 255);

		DrawOutlinedRect(x, y + 1, width, height - 2, background);
		Interfaces::MatSurface()->DrawSetColor(background);
		Interfaces::MatSurface()->DrawFilledRect(x - 1, y, x + width + 1, y + height);
		Interfaces::MatSurface()->DrawSetColor(foreground);
		Interfaces::MatSurface()->DrawFilledRect(x - 1, y, x + int(width * healthNormalized) + 1, y + height);
	}

	static void RenderBoxes(C_CSPlayer* pLocal, C_CSPlayer* pTarget, se::Vector vScreenHead, se::Vector vScreenOrigin)
	{
		using namespace se;
		if (Options::g_iESPShowBoxes == 0)
			return;

		auto isEnemy = pLocal->GetTeamNum() != pTarget->GetTeamNum();
		if (Options::g_iESPShowBoxes == (isEnemy ? 1 : 2))
			return;

		auto allyColor = Options::g_fESPAllyColor;
		auto enemyColor = Options::g_fESPEnemyColor;

		auto height = abs(vScreenHead.y - vScreenOrigin.y);
		auto width = height * 0.65f;
		auto lineColor = isEnemy
			? Color(int(enemyColor[0] * 255.0f), int(enemyColor[1] * 255.0f), int(enemyColor[2] * 255.0f), int(enemyColor[3] * 255.0f))
			: Color(int(allyColor[0] * 255.0f), int(allyColor[1] * 255.0f), int(allyColor[2] * 255.0f), int(allyColor[3] * 255.0f));

		DrawOutlinedRect(int(vScreenHead.x - width / 2), int(vScreenHead.y), int(width), int(height), lineColor);
	}

	static void RenderTop(C_CSPlayer* pLocal, C_CSPlayer* pTarget, se::Vector vScreenHead)
	{
		using namespace se;
		if (Options::g_iESPShowNames == 0)
			return;

		auto isEnemy = pLocal->GetTeamNum() != pTarget->GetTeamNum();
		if (Options::g_iESPShowNames == (isEnemy ? 1 : 2))
			return;

		int y;
		if (Options::g_iESPShowHealth == 0 || Options::g_iESPShowHealth == (isEnemy ? 1 : 2))
			y = int(vScreenHead.y);
		else 
			y = int(vScreenHead.y - 4 * 2); // 4 -> healthBox height

		player_info_t pInfo;
		Interfaces::Engine()->GetPlayerInfo(pTarget->EntIndex(), &pInfo);
		DrawString(int(vScreenHead.x), y, 255, 255, 255, 255, true, pInfo.szName, true);
	}
	
	static void RenderBottom(C_CSPlayer* pLocal, C_CSPlayer* pTarget, se::Vector vScreenOrigin)
	{
		using namespace se;
		if (Options::g_iESPShowDistance == 0 && Options::g_iESPShowWeapon == 0)
			return;

		auto isEnemy = pLocal->GetTeamNum() != pTarget->GetTeamNum();
		auto shouldDrawDistance = Options::g_iESPShowDistance == 3 || Options::g_iESPShowDistance == (isEnemy ? 2 : 1);
		auto shouldDrawWeapon = Options::g_iESPShowWeapon == 3 || Options::g_iESPShowWeapon == (isEnemy ? 2 : 1);
		if (!shouldDrawDistance && !shouldDrawWeapon)
			return;

		auto vOrigin = pTarget->GetOrigin();
		auto dist = pLocal->GetOrigin().DistTo(vOrigin);
		auto pWeapon = pTarget->GetActiveWeapon();
		char szText[36] = "";
		if (pWeapon && shouldDrawWeapon)
		{
			sprintf(szText, "%s", pWeapon->GetReadableName().c_str());
			if (shouldDrawDistance)
				sprintf(szText, "%s (%.0fm)", szText, ceil(dist * 0.0254f));
		}
		else if (shouldDrawDistance)
			sprintf(szText, "%s%.0fm", szText, ceil(dist * 0.0254f));
		DrawString(int(vScreenOrigin.x), int(vScreenOrigin.y), 255, 255, 255, 255, true, szText);
	}

	static void RenderBones(C_CSPlayer* pLocal, C_CSPlayer* pTarget)
	{
		using namespace se;
		if (Options::g_iESPShowBones == 0)
			return;

		auto isEnemy = pLocal->GetTeamNum() != pTarget->GetTeamNum();
		if (Options::g_iESPShowBones == (isEnemy ? 1 : 2))
			return;

		auto pStudioModel = Interfaces::ModelInfo()->GetStudioModel(pTarget->GetModel());
		if (!pStudioModel)
			return;
		
		static matrix3x4_t pBoneToWorldOut[128];
		if (pTarget->SetupBones(pBoneToWorldOut, 128, 256, Interfaces::Engine()->GetLastTimeStamp()))
		{
			for (auto i = 0; i < pStudioModel->numbones; i++)
			{
				auto pBone = pStudioModel->pBone(i);
				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
					continue;

				Vector vBonePos1;
				auto hitbox = pBoneToWorldOut[i];
				if (!Utils::WorldToScreen(Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]), vBonePos1))
					continue;

				Vector vBonePos2;
				hitbox = pBoneToWorldOut[pBone->parent];
				if (!Utils::WorldToScreen(Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]), vBonePos2))
					continue;

				auto allyColor = Options::g_fESPAllyColor;
				auto enemyColor = Options::g_fESPEnemyColor;
				Interfaces::MatSurface()->DrawSetColor(isEnemy
					? Color(int(enemyColor[0] * 255.0f), int(enemyColor[1] * 255.0f), int(enemyColor[2] * 255.0f), int(enemyColor[3] * 255.0f))
					: Color(int(allyColor[0] * 255.0f), int(allyColor[1] * 255.0f), int(allyColor[2] * 255.0f), int(allyColor[3] * 255.0f)));
				Interfaces::MatSurface()->DrawLine(int(vBonePos1.x), int(vBonePos1.y), int(vBonePos2.x), int(vBonePos2.y));
			}
		}
	}

	// Helpers
	static void DrawOutlinedRect(int x, int y, int width, int height, se::Color color)
	{
		using namespace se;
		auto x2 = x + width;
		auto y2 = y + height;
		Interfaces::MatSurface()->DrawSetColor(color);
		Interfaces::MatSurface()->DrawOutlinedRect(x, y, x2, y2);
		Interfaces::MatSurface()->DrawOutlinedRect(x - 1, y - 1, x2 + 1, y2 + 1);
		Interfaces::MatSurface()->DrawSetColor(0, 0, 0, 255);
		Interfaces::MatSurface()->DrawOutlinedRect(x - 2, y - 2, x2 + 2, y2 + 2);
		Interfaces::MatSurface()->DrawOutlinedRect(x + 1, y + 1, x2 - 1, y2 - 1);
	}

	static FontSize DrawString(int x, int y, int r, int g, int b, int a, bool bCenter, const char *pszText, bool bVerticalAlignTop = false)
	{
		if (pszText == nullptr)
			return FontSize { -1, -1 };

		wchar_t szString[64];
		MultiByteToWideChar(CP_UTF8, 0, pszText, -1, szString, 64);

		static se::HFont font;
		static auto fontInitialized = false;
		if (!fontInitialized)
		{
			se::Interfaces::MatSurface()->SetFontGlyphSet(
				font = se::Interfaces::MatSurface()->CreateFont(),
				XorStr("Consolas"), 14,
				FW_MEDIUM, 0, 0,
				static_cast<int>(se::FontFlags::FONTFLAG_OUTLINE)
			);
			fontInitialized = true;
		}

		int iWidth, iHeight;
		se::Interfaces::MatSurface()->GetTextSize(font, szString, iWidth, iHeight);
		se::Interfaces::MatSurface()->DrawSetTextFont(font);
		se::Interfaces::MatSurface()->DrawSetTextPos(x - (bCenter ? iWidth / 2 : 0), y - (bVerticalAlignTop ? iHeight : 0));
		se::Interfaces::MatSurface()->DrawSetTextColor(se::Color(r, g, b, a));
		se::Interfaces::MatSurface()->DrawPrintText(szString, wcslen(szString));
		return FontSize { iWidth, iHeight };
	}
};
