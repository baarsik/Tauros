#pragma once

bool bIsPlanted = false;
float flPlantedTime;
float flExplodesIn;

class C4Timer
{
public:
	static void PaintTraverse_Post()
	{
		if (!Options::g_bC4TimerEnabled || !bIsPlanted)
			return;

		using namespace se;
		auto time = static_cast<int>(flExplodesIn - C_CSPlayer::GetLocalPlayer()->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick);
		int width, height;
		Interfaces::MatSurface()->GetScreenSize(width, height);
		auto textSize1 = GetTextSize(XorStr("EXPLODE IN"));
		auto textSize2 = GetTextSizeHeavy(std::to_string(time).c_str());
		auto y1 = static_cast<int>(height / 1.5f) - 15 - (textSize1.height + textSize2.height) / 2;
		auto y2 = y1 + 30 + textSize1.height + textSize2.height;
		auto x1 = width - 140;
		auto x2 = width;
		Interfaces::MatSurface()->DrawSetColor(0, 0, 0, 200);
		Interfaces::MatSurface()->DrawFilledRect(x1, y1, x2, y2);
		auto x = (x1 + x2) / 2;
		auto y = y1 + 10;
		DrawString(x, y, 255, 255, 255, 255, XorStr("EXPLODE IN"));
		y += textSize1.height + 10;
		DrawStringHeavy(x, y, 255, 255, 255, 255, std::to_string(time).c_str());
	}

	static void OnBombPlanted(C_CSPlayer* planter, int site)
	{
		auto pLocal = C_CSPlayer::GetLocalPlayer();
		flExplodesIn = pLocal->GetTickBase() * se::Interfaces::GlobalVars()->interval_per_tick + se::Interfaces::CVar()->FindVar(XorStr("mp_c4timer"))->GetInt() + 1;
		bIsPlanted = true;
	}

	static void OnBombExploded(C_CSPlayer* planter, int site)
	{
		bIsPlanted = false;
	}

	static void OnRoundStart(int timelimit, int fraglimit, const char* objective)
	{
		bIsPlanted = false;
	}

	static void OnLocalPlayerSpawn(bool inrestart)
	{
		bIsPlanted = false;
	}
private:
	static FontSize GetTextSize(const char* pszText)
	{
		if (pszText == nullptr)
			return FontSize{ -1, -1 };

		static se::HFont font;
		static auto fontInitialized = false;
		if (!fontInitialized)
		{
			se::Interfaces::MatSurface()->SetFontGlyphSet(
				font = se::Interfaces::MatSurface()->CreateFont(),
				XorStr("Tahoma"), 16,
				FW_NORMAL, 0, 0,
				static_cast<int>(se::FontFlags::FONTFLAG_NONE)
			);
			fontInitialized = true;
		}

		int iWidth, iHeight;
		wchar_t szString[64];
		MultiByteToWideChar(CP_UTF8, 0, pszText, -1, szString, 64);
		se::Interfaces::MatSurface()->GetTextSize(font, szString, iWidth, iHeight);
		return FontSize { iWidth, iHeight };
	}

	static FontSize GetTextSizeHeavy(const char* pszText)
	{
		if (pszText == nullptr)
			return FontSize{ -1, -1 };

		static se::HFont font;
		static auto fontInitialized = false;
		if (!fontInitialized)
		{
			se::Interfaces::MatSurface()->SetFontGlyphSet(
				font = se::Interfaces::MatSurface()->CreateFont(),
				XorStr("Tahoma"), 36,
				FW_HEAVY, 0, 0,
				static_cast<int>(se::FontFlags::FONTFLAG_NONE)
			);
			fontInitialized = true;
		}

		int iWidth, iHeight;
		wchar_t szString[64];
		MultiByteToWideChar(CP_UTF8, 0, pszText, -1, szString, 64);
		se::Interfaces::MatSurface()->GetTextSize(font, szString, iWidth, iHeight);
		return FontSize{ iWidth, iHeight };
	}

	static FontSize DrawString(int x, int y, int r, int g, int b, int a, const char* pszText)
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
				XorStr("Tahoma"), 16,
				FW_NORMAL, 0, 0,
				static_cast<int>(se::FontFlags::FONTFLAG_NONE)
			);
			fontInitialized = true;
		}

		int iWidth, iHeight;
		se::Interfaces::MatSurface()->GetTextSize(font, szString, iWidth, iHeight);
		se::Interfaces::MatSurface()->DrawSetTextFont(font);
		se::Interfaces::MatSurface()->DrawSetTextPos(x - iWidth / 2, y);
		se::Interfaces::MatSurface()->DrawSetTextColor(se::Color(r, g, b, a));
		se::Interfaces::MatSurface()->DrawPrintText(szString, wcslen(szString));
		return FontSize{ iWidth, iHeight };
	}

	static FontSize DrawStringHeavy(int x, int y, int r, int g, int b, int a, const char* pszText)
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
				XorStr("Tahoma"), 36,
				FW_HEAVY, 0, 0,
				static_cast<int>(se::FontFlags::FONTFLAG_NONE)
			);
			fontInitialized = true;
		}

		int iWidth, iHeight;
		se::Interfaces::MatSurface()->GetTextSize(font, szString, iWidth, iHeight);
		se::Interfaces::MatSurface()->DrawSetTextFont(font);
		se::Interfaces::MatSurface()->DrawSetTextPos(x - iWidth / 2, y);
		se::Interfaces::MatSurface()->DrawSetTextColor(se::Color(r, g, b, a));
		se::Interfaces::MatSurface()->DrawPrintText(szString, wcslen(szString));
		return FontSize{ iWidth, iHeight };
	}
};