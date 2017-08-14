// Basic stuff
#include "VFTableHook.hpp"
#include "MaterialHelper.hpp"
#include "SignatureHelper.hpp"
#include "Hooks.hpp"
#include "Options.hpp"
#include "Utils.hpp"
#include "XorStr.hpp"

// GUI related
#include "DrawManager.hpp"
#include "ImGUI/imgui.h"
#include "ImGUI/DX9/imgui_impl_dx9.h"
#include "GUI.hpp"

// Hacks
#include "Hacks/AimAssist.hpp"
#include "Hacks/AutoAccept.hpp"
#include "Hacks/AutoPistol.hpp"
#include "Hacks/Bhop.hpp"
#include "Hacks/C4Timer.hpp"
#include "Hacks/Chams.hpp"
#include "Hacks/DamageInformer.hpp"
#include "Hacks/ESP.hpp"
#include "Hacks/VisualMisc.hpp"
#include "Hacks/RankRevealer.hpp"
#include "Hacks/RCS.hpp"
#include "Hacks/SkinChanger.hpp"
#include "Hacks/Trigger.hpp"

// Helper classes
#include "EventListener.hpp"

using namespace std;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern HMODULE g_hLib;
extern HWND g_hWnd;

namespace Hooks
{
    unique_ptr<VFTableHook>            g_pD3DDevice9Hook = nullptr;
	unique_ptr<VFTableHook>            g_pClientHook = nullptr;
    unique_ptr<VFTableHook>            g_pClientModeHook = nullptr;
    unique_ptr<VFTableHook>            g_pMatSurfaceHook = nullptr;
	unique_ptr<VFTableHook>            g_pVGUIPanelHook = nullptr;
	unique_ptr<VFTableHook>            g_pModelRenderHook = nullptr;
	unique_ptr<VFTableHook>            g_pDrawModelExecuteHook = nullptr;
	unique_ptr<VFTableHook>            g_pEventManagerHook = nullptr;

    unique_ptr<DrawManager>            g_pRenderer = nullptr;

    EndScene_t                         g_fnOriginalEndScene = nullptr;
    Reset_t                            g_fnOriginalReset = nullptr;
    CreateMove_t                       g_fnOriginalCreateMove = nullptr;
	PlaySound_t                        g_fnOriginalPlaySound = nullptr;
	PaintTraverse_t                    g_fnOriginalPaintTraverse = nullptr;	
	FrameStageNotify_t                 g_fnOriginalFrameStageNotify = nullptr;
	OverrideView_t                     g_fnOriginalOverrideView = nullptr;
	DrawModelExecute_t                 g_fnOriginalDrawModelExecute = nullptr;
	OverrideMouseInput_t               g_fnOriginalOverrideMouseInput = nullptr;

    WNDPROC                            g_pOldWindowProc = nullptr;

	GUI                                g_Gui;
	unique_ptr<EventListener>          g_EventListener = nullptr;

    bool                               g_vecPressedKeys[256] = {};
    bool                               g_bWasInitialized = false;

	void Initialize()
    {
		//Find CSGO main window
		while (!g_hWnd)
		{
			g_hWnd = FindWindowA(XorStr("Valve001"), nullptr);
			Sleep(200);
		}

        NetvarManager::Instance()->CreateDatabase();
        NetvarManager::Instance()->Dump(Utils::GetDllDir() + XorStr("netvar_dump.txt"));

        g_pD3DDevice9Hook = make_unique<VFTableHook>(SignatureHelper::D3DDevice());
		g_pClientHook = make_unique<VFTableHook>(se::Interfaces::Client());
        g_pClientModeHook = make_unique<VFTableHook>(se::Interfaces::ClientMode());
        g_pMatSurfaceHook = make_unique<VFTableHook>(se::Interfaces::MatSurface());
	    g_pVGUIPanelHook = make_unique<VFTableHook>(se::Interfaces::VGUIPanel());
		g_pModelRenderHook = make_unique<VFTableHook>(se::Interfaces::ModelRender());
		g_pEventManagerHook = make_unique<VFTableHook>(se::Interfaces::EventManager());

        g_pOldWindowProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Hooked_WndProc)));

        g_fnOriginalReset = g_pD3DDevice9Hook->Hook(16, Hooked_Reset);                                                                   // IDirect3DDevice9::Reset
        g_fnOriginalEndScene = g_pD3DDevice9Hook->Hook(42, Hooked_EndScene);                                                             // IDirect3DDevice9::EndScene
		g_fnOriginalFrameStageNotify = g_pClientHook->Hook(36, reinterpret_cast<FrameStageNotify_t>(Hooked_FrameStageNotify));           // Client::FrameStageNotify
		g_fnOriginalOverrideView = g_pClientModeHook->Hook(18, reinterpret_cast<OverrideView_t>(Hooked_OverrideView));                   // IClientMode::OverrideView
        g_fnOriginalCreateMove = g_pClientHook->Hook(21, reinterpret_cast<CreateMove_t>(Hooked_CreateMove_Proxy));                   // IClientMode::CreateMove
		g_fnOriginalPlaySound = g_pMatSurfaceHook->Hook(82, reinterpret_cast<PlaySound_t>(Hooked_PlaySound));                            // ISurface::PlaySound
	    g_fnOriginalPaintTraverse = g_pVGUIPanelHook->Hook(41, reinterpret_cast<PaintTraverse_t>(Hooked_PaintTraverse));                 // IPanel::PaintTraverse
		g_fnOriginalDrawModelExecute = g_pModelRenderHook->Hook(21, reinterpret_cast<DrawModelExecute_t>(Hooked_DrawModelExecute));      // IVModelRender::DrawModelExecute
		g_fnOriginalOverrideMouseInput = g_pClientModeHook->Hook(23, reinterpret_cast<OverrideMouseInput_t>(Hooked_OverrideMouseInput)); // IClientMode::OverrideMouseInput

		g_EventListener = make_unique<EventListener>();
    }

    void Restore()
    {
        //Restore the WindowProc
        SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_pOldWindowProc));

        g_pRenderer->InvalidateObjects();

		// Delete listener
		g_EventListener.reset();

        //Remove the hooks
        g_pD3DDevice9Hook->RestoreTable();
		g_pClientHook->RestoreTable();
        g_pClientModeHook->RestoreTable();
        g_pMatSurfaceHook->RestoreTable();
		g_pVGUIPanelHook->RestoreTable();
		g_pModelRenderHook->RestoreTable();
		g_pEventManagerHook->RestoreTable();
    }

    void GUI_Init(IDirect3DDevice9* pDevice)
    {
        //Initializes the GUI and the renderer
        ImGui_ImplDX9_Init(g_hWnd, pDevice);
		g_Gui = GUI(pDevice);
        g_pRenderer = make_unique<DrawManager>(pDevice);
        g_pRenderer->CreateObjects();
        g_bWasInitialized = true;
    }

	HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
	{
		using namespace se;
		if (!g_bWasInitialized)
		{
			GUI_Init(pDevice);
		}
		else
		{
			g_Gui.UpdateCursorVisibility();
			ImGui_ImplDX9_NewFrame();
			g_Gui.Show();

			g_pRenderer->BeginRendering();
			ImGui::Render();
			g_pRenderer->EndRendering();
		}

		return g_fnOriginalEndScene(pDevice);
	}

	HRESULT __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		if (!g_bWasInitialized)
			return g_fnOriginalReset(pDevice, pPresentationParameters);

		// Device is on LOST state.
		ImGui_ImplDX9_InvalidateDeviceObjects();
		g_pRenderer->InvalidateObjects();

		// Call original Reset.
		auto hrOriginalReset = g_fnOriginalReset(pDevice, pPresentationParameters);

		g_pRenderer->CreateObjects();
		ImGui_ImplDX9_CreateDeviceObjects();
		return hrOriginalReset;
	}

    void __fastcall Hooked_PaintTraverse(se::IPanel* pPanel, void * edx, se::VPANEL vguiPanel, bool forceRepaint, bool allowForce)
    {
	    g_fnOriginalPaintTraverse(pPanel, vguiPanel, forceRepaint, allowForce);
		if (Options::g_bCleanScreenshot && se::Interfaces::Engine()->IsTakingScreenshot())
			return;

		static unsigned int overlayPanel = 0;
		if (overlayPanel == 0)
		{
			if (!strstr(se::Interfaces::VGUIPanel()->GetName(vguiPanel), XorStr("MatSystemTopPanel")))
				return;

			overlayPanel = vguiPanel;
		}

		if (overlayPanel != vguiPanel)
			return;

		if (!se::Interfaces::Engine()->IsInGame())
			return;

		ESP::PaintTraverse_Post();
		DamageInformer::PaintTraverse_Post();
		C4Timer::PaintTraverse_Post();
	}

    LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uMsg)
		{
            case WM_LBUTTONDOWN:
                g_vecPressedKeys[VK_LBUTTON] = true;
                break;
            case WM_LBUTTONUP:
                g_vecPressedKeys[VK_LBUTTON] = false;
                break;
            case WM_RBUTTONDOWN:
                g_vecPressedKeys[VK_RBUTTON] = true;
                break;
            case WM_RBUTTONUP:
                g_vecPressedKeys[VK_RBUTTON] = false;
                break;
            case WM_KEYDOWN:
                g_vecPressedKeys[wParam] = true;
                break;
            case WM_KEYUP:
                g_vecPressedKeys[wParam] = false;
                break;
            default: break;
        }

		g_Gui.CheckToggle(g_vecPressedKeys, VK_INSERT);
		g_Gui.CheckToggle(g_vecPressedKeys, VK_F11);

		AutoPistol::CheckToggle(g_vecPressedKeys);

        if(g_bWasInitialized && Options::g_bMainWindowOpen && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
            return true;

        return CallWindowProc(g_pOldWindowProc, hWnd, uMsg, wParam, lParam);
    }

    void __stdcall Hooked_CreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
    {
	    g_fnOriginalCreateMove(se::Interfaces::Client(), sequence_number, input_sample_frametime, active);
        auto pLocal = C_CSPlayer::GetLocalPlayer();

		auto cmd = se::Interfaces::Input()->GetUserCmd(sequence_number);
		auto verified = se::Interfaces::Input()->GetVerifiedUserCmd(sequence_number);

		if (!cmd || !verified)
			return;

		Bhop::CreateMove_Post(pLocal, cmd);
		RCS::CreateMove_Post(pLocal, cmd);
		Trigger::CreateMove_Post(pLocal, cmd);
		AimAssist::CreateMove_Post(pLocal, cmd);
		AutoPistol::CreateMove_Post(pLocal, cmd);
		RankRevealer::CreateMove_Post(pLocal, cmd);

		SignatureHelper::SetClanTag("Cerberus", "Cerberus");

		verified->m_cmd = *cmd;
		verified->m_crc = cmd->GetChecksum();
    }

	__declspec(naked) void __stdcall Hooked_CreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active)
	{
		__asm
		{
			push ebp
			mov  ebp, esp
			push ebx
			lea  ecx, [esp]
			push ecx
			push dword ptr[active]
			push dword ptr[input_sample_frametime]
			push dword ptr[sequence_number]
			call Hooks::Hooked_CreateMove
			pop  ebx
			pop  ebp
			retn 0Ch
		}
	}

	void __fastcall Hooked_FrameStageNotify(void* ecx, void* edx, se::ClientFrameStage_t stage)
    {
		NoSmoke::FrameStageNotify_Pre(stage);
		NoFlash::FrameStageNotify_Pre(stage);
		RCS::FrameStageNotify_Pre(stage);
		SkinChanger::FrameStageNotify_Pre(stage);

		g_fnOriginalFrameStageNotify(ecx, stage);

		RCS::FrameStageNotify_Post(stage);
    }
	
	void __fastcall Hooked_OverrideView(void* ecx, void* edx, se::CViewSetup* pViewSetup)
	{
		RCS::OverrideView_Pre(pViewSetup);

		g_fnOriginalOverrideView(ecx, pViewSetup);
	}

	void __fastcall Hooked_DrawModelExecute(void* ecx, void* edx, se::IMatRenderContext* ctx, const se::DrawModelState_t &state, const se::ModelRenderInfo_t &pInfo, se::matrix3x4_t *pCustomBoneToWorld)
	{
		g_pModelRenderHook->Unhook(21); // Prevent infinite recursive loop
		MatHelper.Initialize();

		if (!Options::g_bCleanScreenshot || !se::Interfaces::Engine()->IsTakingScreenshot())
		{
			Chams::DrawModelExecute_Pre(ecx, ctx, state, pInfo, pCustomBoneToWorld);
			Hands::DrawModelExecute_Pre(ecx, ctx, state, pInfo, pCustomBoneToWorld);
		}
		
		g_fnOriginalDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
		se::Interfaces::ModelRender()->ForcedMaterialOverride(nullptr);
		g_pModelRenderHook->Hook(21, reinterpret_cast<DrawModelExecute_t>(Hooked_DrawModelExecute));
	}

	void __stdcall Hooked_OverrideMouseInput(float* x, float* y)
	{
		g_fnOriginalOverrideMouseInput(se::Interfaces::ClientMode(), x, y);

		AimAssist::OverrideMouseInput_Post(x, y);
	}

	void __stdcall Hooked_PlaySound(const char* szFileName)
	{
		g_fnOriginalPlaySound(se::Interfaces::MatSurface(), szFileName);

		AutoAccept::PlaySound_Post(szFileName);
	}
}