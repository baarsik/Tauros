#pragma once
#include "ImGUI/IconsFontAwesome.h"

enum Window : int
{
	WMain = 0,
	WAim,
	WVisuals,
	WTrigger,
	WSkin,
	WMisc,
	WConfigs
};

std::vector<std::string> TargetsAffected = { XorStr("None"), XorStr("Allies"), XorStr("Enemies"), XorStr("Allies and enemies") };

class GUI
{
public:
	GUI()
	{
		clientWidth = 1024;
		clientHeight = 768;
		openedWindow = WMain;
	}

	explicit GUI(IDirect3DDevice9* pDevice)
	{
		UpdateSize(pDevice);
		openedWindow = WMain;

		auto& io = ImGui::GetIO();
		auto fontDir = std::string(Utils::GetDllDir()) + XorStr("fonts\\");
		io.FontDefault = io.Fonts->AddFontFromFileTTF((fontDir + XorStr("gost-a.ttf")).c_str(), 14.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF((fontDir + XorStr("fontawesome-webfont.ttf")).c_str(), 16.0f, &icons_config, icons_ranges);
	}

	void SetStyle() const
	{
		auto style = &ImGui::GetStyle();

		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 0.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		SetButtonStyle();
		if (Options::g_iGuiSchema == 0) // Dark
		{
			style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
			style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.7f);
			style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
			style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
			style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
			style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 0.75f);
			style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
			style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2058f, 0.3066f, 0.4877f, 1.0f);
			style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1734f, 0.2995f, 0.5270f, 1.00f);
			style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.2330f, 0.3125f, 0.4545f, 1.00f);
			style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
			style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.2058f, 0.3066f, 0.4877f, 1.0f);
			style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2330f, 0.3125f, 0.4545f, 1.00f);
			style->Colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.21f, 1.00f);
			style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
			style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
			style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
			style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
			style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
		}
		else if (Options::g_iGuiSchema == 1) // White
		{
			style->Colors[ImGuiCol_Text] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
			style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.f);
			style->Colors[ImGuiCol_WindowBg] = ImVec4(1.f, 1.f, 1.f, 0.35f);
			style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 0.35f);
			style->Colors[ImGuiCol_PopupBg] = ImVec4(0.95f, 0.95f, 0.95f, 0.35f);
			style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
			style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
			style->Colors[ImGuiCol_FrameBg] = ImVec4(1.f, 1.f, 1.f, 1.f);
			style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(40.f / 255.f, 96.f / 255.f, 144.f / 255.f, 1.f);
			style->Colors[ImGuiCol_FrameBgActive] = ImVec4(40.f / 255.f, 96.f / 255.f, 144.f / 255.f, 1.f);
			style->Colors[ImGuiCol_TitleBg] = ImVec4(1.f, 1.f, 1.f, 1.f);
			style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.f, 1.f, 1.f, 1.f);
			style->Colors[ImGuiCol_TitleBgActive] = ImVec4(1.f, 1.f, 1.f, 1.f);
			style->Colors[ImGuiCol_MenuBarBg] = ImVec4(1.f, 1.f, 1.f, 1.f);
			style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(1.f, 1.f, 1.f, 1.f);
			style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2f, 122.f / 255.f, 183.f / 255.f, 1.f);
			style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(40.f / 255.f, 96.f / 255.f, 144.f / 255.f, 1.f);
			style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(40.f / 255.f, 96.f / 255.f, 144.f / 255.f, 1.f);
			style->Colors[ImGuiCol_ComboBg] = ImVec4(1.f, 1.f, 1.f, 1.f);
			style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.2f, 122.f / 255.f, 183.f / 255.f, 1.f);
			style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(40.f / 255.f, 96.f / 255.f, 144.f / 255.f, 1.f);
			style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.f);
			style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.f);
			style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.f);
			style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.f);
			style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.f);
			style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.f);
			style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.f);
			style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
			style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
			style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.f);
			style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.f, 0.00f, 1.f);
			style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.f, 0.00f, 1.f);
			style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.f, 0.00f, 0.43f);
			style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.f, 0.98f, 0.95f, 0.73f);
		}
	}

	void SetButtonStyle(bool uniqueStyle = false) const
	{
		auto style = &ImGui::GetStyle();
		if (Options::g_iGuiSchema == 0)
		{
			if (uniqueStyle)
			{
				style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
				style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
				style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
			}
			else
			{
				style->Colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.21f, 1.00f);
				style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
				style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.3f, 0.3f, 0.33f, 1.00f);
			}
		}
		else if (Options::g_iGuiSchema == 1)
		{
			if (uniqueStyle)
			{
				style->Colors[ImGuiCol_Button] = ImVec4(1.f, 1.f, 1.f, 1.f);
				style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.95f, 0.95f, 0.95f, 1.f);
				style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.95f, 0.95f, 0.95f, 1.f);
				style->Colors[ImGuiCol_Text] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
			}
			else
			{
				style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 122.f / 255.f, 183.f / 255.f, 1.f);
				style->Colors[ImGuiCol_ButtonHovered] = ImVec4(40.f / 255.f, 96.f / 255.f, 144.f / 255.f, 1.f);
				style->Colors[ImGuiCol_ButtonActive] = ImVec4(40.f / 255.f, 96.f / 255.f, 144.f / 255.f, 1.f);
				style->Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
			}
		}
	}

	void DrawSpecialText(std::string text, std::string textBefore, bool sameLine = false, bool defaultColor = false) const
	{
		auto style = &ImGui::GetStyle();
		ImVec4 color;
		if (defaultColor)
		{
			ImGui::Text(text.c_str());
		}
		else
		{
			if (Options::g_iGuiSchema == 0)        color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
			else if (Options::g_iGuiSchema == 1)   color = ImVec4(1.0f, 0.25f, 0.0f, 1.0f);
			ImGui::TextColored(color, text.c_str());
		}
		if (sameLine) ImGui::SameLine(style->WindowPadding.x + ImGui::CalcTextSize((textBefore + text).c_str()).x);
	}

	void DrawSpecialText(std::string text, std::string textBefore, bool sameLine, ImVec4 color) const
	{
		auto style = &ImGui::GetStyle();
		ImGui::TextColored(color, text.c_str());
		if (sameLine) ImGui::SameLine(style->WindowPadding.x + ImGui::CalcTextSize((textBefore + text).c_str()).x);
	}

	void Show()
	{
		if (!Options::g_bMainWindowOpen)
			return;

		SetStyle();
		auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
		auto style = &ImGui::GetStyle();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(static_cast<float>(clientWidth), 70));
		ImGui::Begin(XorStr("Cerberus.Tauros"), nullptr, flags | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		for (int i = WMain; i <= WConfigs; i++)
		{
			SetButtonStyle(openedWindow == i);
			char* text = XorStr("Unknown");
			if (i == WMain)
				text = XorStr(ICON_FA_INFO "  Main");
			else if (i == WAim)
				text = XorStr(ICON_FA_WHEELCHAIR "  Aim");
			else if (i == WVisuals)
				text = XorStr(ICON_FA_EYE "  Visuals");
			else if (i == WTrigger)
				text = XorStr(ICON_FA_CROSSHAIRS "  Trigger");
			else if (i == WSkin)
				text = XorStr(ICON_FA_PAINT_BRUSH "  Skin Changer");
			else if (i == WMisc)
				text = XorStr(ICON_FA_COGS "  Misc");
			else if (i == WConfigs)
				text = XorStr(ICON_FA_SLIDERS "  Configs");
			
			if (ImGui::Button(text, ImVec2(ImGui::CalcTextSize(text).x + 32, 24)))
				openedWindow = Window(i);

			if (i != WConfigs) ImGui::SameLine();
		}
		ImGui::End();

		SetStyle();
		ImGui::SetNextWindowPos(ImVec2(0, 70));
		ImGui::SetNextWindowSize(ImVec2(static_cast<float>(clientWidth), clientHeight - 70.0f));
		ImGui::Begin(XorStr(""), &Options::g_bMainWindowOpen, flags | ImGuiWindowFlags_NoTitleBar);
		if (openedWindow == WMain)
		{
			DrawSpecialText(XorStr("Cerberus"), XorStr(""), true, false);
			DrawSpecialText(XorStr("."), XorStr("Cerberus"), true, true);
			DrawSpecialText(XorStr("Tauros"), XorStr("Cerberus. "), true, ImVec4(0.2f, 122.f / 255.f, 183.f / 255.f, 1.f));
			DrawSpecialText(std::string(XorStr("(")) + XorStr(__DATE__) + XorStr(")"), XorStr("Cerberus.Tauros"), false, true);
			ImGui::Text(XorStr(u8"Coded by Барс"));
			DrawSpecialText(XorStr("\nCredits:"), XorStr(""), false, false);
			ImGui::Text(XorStr("MarkHC for his CSGOSimple base"));
			ImGui::Text(XorStr("Unknowncheats community for lots of code snippets"));
#if DEBUG

#endif
		}
		else if (openedWindow == WAim)
		{
			ImGui::Columns(3, nullptr, false);
			DrawSpecialText(XorStr("Aim Assist"), XorStr(""), false, false);
			ImGui::Checkbox(XorStr("Enabled"), &Options::g_bAimAssistEnabled);
			ImGui::Checkbox(XorStr("Friendly fire"), &Options::g_bAimAssistFriendlyFire);
			ImGui::Checkbox(XorStr("Ignore obstacles"), &Options::g_bAimAssistIgnoreObstacles);
			ImGui::Checkbox(XorStr("Auto aim"), &Options::g_bAimAssistAutoShoot);
			ImGui::Checkbox(XorStr("Lock mouse"), &Options::g_bAimAssistLockMouse);
			ImGui::SliderFloat(XorStr("FOV"), &Options::g_fAimAssistFov, 0.0f, 60.0f, XorStr("%.2f"));
			ImGui::SliderFloat(XorStr("Smooth"), &Options::g_fAimAssistSmooth, 0.022f, 1.5f, XorStr("%.4f"));
			ImGui::SliderFloat(XorStr("Vertical smooth multiplier"), &Options::g_fAimAssistVerticalSmoothMultiplier, 1.0f, 5.0f, XorStr("%.2f"));
		}
		else if (openedWindow == WVisuals)
		{
			ImGui::Columns(3, nullptr, false);
			DrawSpecialText(XorStr("ESP"), XorStr(""), false, false);
			ImGui::Checkbox(XorStr("Enabled"), &Options::g_bESPEnabled);
			ImGui::Combo(XorStr("Boxes"), &Options::g_iESPShowBoxes, TargetsAffected);
			ImGui::Combo(XorStr("Health"), &Options::g_iESPShowHealth, TargetsAffected);
			ImGui::Combo(XorStr("Bones"), &Options::g_iESPShowBones, TargetsAffected);
			ImGui::Combo(XorStr("Names"), &Options::g_iESPShowNames, TargetsAffected);
			ImGui::Combo(XorStr("Weapons"), &Options::g_iESPShowWeapon, TargetsAffected);
			ImGui::Combo(XorStr("Distance"), &Options::g_iESPShowDistance, TargetsAffected);
			ImGui::Text(XorStr(""));
			ImGui::ColorEdit3(XorStr("Enemy color"), Options::g_fESPEnemyColor);
			ImGui::ColorEdit3(XorStr("Ally color"), Options::g_fESPAllyColor);

			ImGui::NextColumn();
			DrawSpecialText(XorStr("Chams"), XorStr(""), false, false);
			ImGui::Checkbox(XorStr("Enabled "), &Options::g_bChamsEnabled);
			ImGui::Combo(XorStr("Visible material"), &Options::g_iChamsVisibleMaterialId, MatHelper.GetMaterialNames(false));
			ImGui::ColorEdit3(XorStr("Visible color"), Options::g_fChamsVisibleColor);
			ImGui::Combo(XorStr("Invisible material"), &Options::g_iChamsInvisibleMaterialId, MatHelper.GetMaterialNames(true));
			ImGui::ColorEdit3(XorStr("Invisible color"), Options::g_fChamsInvisibleColor);

			ImGui::NextColumn();
			DrawSpecialText(XorStr("Misc"), XorStr(""), false, false);
			ImGui::Checkbox(XorStr("Clean screenshots"), &Options::g_bCleanScreenshot);
			ImGui::Checkbox(XorStr("Show damage"), &Options::g_bDamageInformerEnabled);
			ImGui::Checkbox(XorStr("No smoke"), &Options::g_bNoSmokeEnabled);
			ImGui::Checkbox(XorStr("No flash"), &Options::g_bNoFlashEnabled);
			ImGui::SliderFloat(XorStr("Max flash"), &Options::g_fNoFlashAmount, 0.0f, 100.0f, XorStr("%.0f%%"));
			ImGui::Combo(XorStr("Hands style"), &Options::g_iHandsDisplay, Options::g_szHandsDisplay, IM_ARRAYSIZE(Options::g_szHandsDisplay));
		}
		else if (openedWindow == WTrigger)
		{
			ImGui::Columns(3, nullptr, false);
			ImGui::Checkbox(XorStr("Enabled"), &Options::g_bTriggerEnabled);
			ImGui::Combo(XorStr("Trigger key"), &Options::g_iTriggerKey, Options::Keys, IM_ARRAYSIZE(Options::Keys));
			ImGui::Checkbox(XorStr("Always active"), &Options::g_bTriggerAlwaysActive);
			ImGui::Checkbox(XorStr("Friendly fire"), &Options::g_bTriggerFriendlyFire);
		}
		else if (openedWindow == WSkin)
		{
			ImGui::Columns(3, nullptr, false);
			ImGui::Checkbox(XorStr("Enabled"), &Options::g_bSkinChangerEnabled);
		}
		else if (openedWindow == WMisc)
		{
			ImGui::Columns(3, nullptr, false);
			ImGui::Checkbox(XorStr("No recoil"), &Options::g_bRCSEnabled);
			ImGui::Checkbox(XorStr("Bunny Hop"), &Options::g_bBHopEnabled);
			ImGui::Checkbox(XorStr("Auto Accept"), &Options::g_bAutoAcceptEnabled);
		}
		else if (openedWindow == WConfigs)
		{
			ImGui::Columns(3, nullptr, false);
			ImGui::Combo(XorStr("GUI Schema"), &Options::g_iGuiSchema, Options::g_szGuiSchema, IM_ARRAYSIZE(Options::g_szGuiSchema));
		}
		ImGui::End();
	}

	void CheckToggle(bool vecPressedKeys[256], short key) const
	{
		static auto isDown = false;
		static auto isClicked = false;
		if (vecPressedKeys[key])
		{
			isClicked = false;
			isDown = true;
		}
		else if (!vecPressedKeys[key] && isDown)
		{
			isClicked = true;
			isDown = false;
		}
		else
		{
			isClicked = false;
			isDown = false;
		}

		if (isClicked) 
		{
			Options::g_bMainWindowOpen = !Options::g_bMainWindowOpen;
			static auto cl_mouseenable = se::Interfaces::CVar()->FindVar(XorStr("cl_mouseenable"));
			cl_mouseenable->SetValue(!Options::g_bMainWindowOpen);
		}
	}

	void UpdateCursorVisibility() const
	{
		ImGui::GetIO().MouseDrawCursor = Options::g_bMainWindowOpen;
	}

	void UpdateSize(IDirect3DDevice9* pDevice)
	{
		D3DDEVICE_CREATION_PARAMETERS cparams;
		pDevice->GetCreationParameters(&cparams);

		RECT rect;
		GetWindowRect(cparams.hFocusWindow, &rect);
		clientWidth = rect.right - rect.left;
		clientHeight = rect.bottom - rect.top;
	}
private:
	int clientWidth;
	int clientHeight;
	Window openedWindow;
};