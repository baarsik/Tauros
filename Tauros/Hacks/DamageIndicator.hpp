#pragma once
#include "Options.hpp"
#include "Utils.hpp"

struct DamageIndicator_t
{
	int iDamage;
	int iDrawPosOffset;
	bool bInitialized;
	float flEraseTime;
	float flLastUpdate;
	C_CSPlayer * Player;
	se::Vector Position;
};
std::vector<DamageIndicator_t> g_vecDamageIndicator;

class DamageIndicator
{
public:
	static void PaintTraverse_Post()
	{
		using namespace se;
		auto pLocal = C_CSPlayer::GetLocalPlayer();
		auto flCurrentTime = pLocal->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick;
		for (size_t i = 0; i < g_vecDamageIndicator.size(); i++)
		{
			if (g_vecDamageIndicator[i].flEraseTime < flCurrentTime)
			{
				g_vecDamageIndicator.erase(g_vecDamageIndicator.begin() + i);
				continue;
			}

			if (!g_vecDamageIndicator[i].bInitialized)
			{
				g_vecDamageIndicator[i].Position = Utils::GetEntityBone(g_vecDamageIndicator[i].Player, HEAD_0);
				g_vecDamageIndicator[i].Position.z -= g_vecDamageIndicator[i].iDrawPosOffset;
				g_vecDamageIndicator[i].bInitialized = true;
			}

			if (flCurrentTime - g_vecDamageIndicator[i].flLastUpdate > 0.0001f)
			{
				g_vecDamageIndicator[i].Position.z -= 0.1f * (flCurrentTime - g_vecDamageIndicator[i].flEraseTime);
				g_vecDamageIndicator[i].flLastUpdate = flCurrentTime;
			}

			Vector vScreenPosition;
			if (!Utils::WorldToScreen(g_vecDamageIndicator[i].Position, vScreenPosition))
				continue;

			if (g_vecDamageIndicator[i].iDamage >= 100)
				DrawStringExtraBold(static_cast<int>(vScreenPosition.x), static_cast<int>(vScreenPosition.y), 255, 75, 75, 255, true, std::to_string(g_vecDamageIndicator[i].iDamage).c_str());
			else
				DrawString(static_cast<int>(vScreenPosition.x), static_cast<int>(vScreenPosition.y), 55, 175, 255, 255, true, std::to_string(g_vecDamageIndicator[i].iDamage).c_str());
		}
	}

	static void FireEvent_Post(se::IGameEvent* event)
	{
		using namespace se;
		if (!Options::g_bDamageIndicatorEnabled)
			return;

		auto pLocal = C_CSPlayer::GetLocalPlayer();
		auto hurt = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"))));
		auto attacker = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("attacker"))));

		if (hurt != pLocal && attacker == pLocal)
		{
			auto drawPosOffset = 0;
			for (auto const& indicator : g_vecDamageIndicator)
			{
				if (indicator.Player == hurt)
					drawPosOffset += 16;
			}
			DamageIndicator_t DmgIndicator;
			DmgIndicator.iDamage = event->GetInt("dmg_health");
			DmgIndicator.iDrawPosOffset = drawPosOffset;
			DmgIndicator.Player = hurt;
			DmgIndicator.flEraseTime = pLocal->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick + 3.f;
			DmgIndicator.bInitialized = false;
			g_vecDamageIndicator.push_back(DmgIndicator);
		}
	}
private:
	static FontSize DrawString(int x, int y, int r, int g, int b, int a, bool bCenter, const char *pszText)
	{
		if (pszText == nullptr)
			return FontSize{ -1, -1 };

		wchar_t szString[64];
		MultiByteToWideChar(CP_UTF8, 0, pszText, -1, szString, 64);

		static se::HFont font;
		static auto fontInitialized = false;
		if (!fontInitialized)
		{
			se::Interfaces::MatSurface()->SetFontGlyphSet(
				font = se::Interfaces::MatSurface()->CreateFont(),
				XorStr("Tahoma"), 18,
				FW_BOLD, 0, 0,
				static_cast<int>(se::FontFlags::FONTFLAG_OUTLINE)
			);
			fontInitialized = true;
		}

		int iWidth, iHeight;
		se::Interfaces::MatSurface()->GetTextSize(font, szString, iWidth, iHeight);
		se::Interfaces::MatSurface()->DrawSetTextFont(font);
		se::Interfaces::MatSurface()->DrawSetTextPos(x - (bCenter ? iWidth / 2 : 0), y);
		se::Interfaces::MatSurface()->DrawSetTextColor(se::Color(r, g, b, a));
		se::Interfaces::MatSurface()->DrawPrintText(szString, wcslen(szString));
		return FontSize{ iWidth, iHeight };
	}

	static FontSize DrawStringExtraBold(int x, int y, int r, int g, int b, int a, bool bCenter, const char *pszText)
	{
		if (pszText == nullptr)
			return FontSize{ -1, -1 };

		wchar_t szString[64];
		MultiByteToWideChar(CP_UTF8, 0, pszText, -1, szString, 64);

		static se::HFont font;
		static auto fontInitialized = false;
		if (!fontInitialized)
		{
			se::Interfaces::MatSurface()->SetFontGlyphSet(
				font = se::Interfaces::MatSurface()->CreateFont(),
				XorStr("Tahoma"), 20,
				FW_HEAVY, 0, 0,
				static_cast<int>(se::FontFlags::FONTFLAG_OUTLINE)
			);
			fontInitialized = true;
		}

		int iWidth, iHeight;
		se::Interfaces::MatSurface()->GetTextSize(font, szString, iWidth, iHeight);
		se::Interfaces::MatSurface()->DrawSetTextFont(font);
		se::Interfaces::MatSurface()->DrawSetTextPos(x - (bCenter ? iWidth / 2 : 0), y);
		se::Interfaces::MatSurface()->DrawSetTextColor(se::Color(r, g, b, a));
		se::Interfaces::MatSurface()->DrawPrintText(szString, wcslen(szString));
		return FontSize{ iWidth, iHeight };
	}
};