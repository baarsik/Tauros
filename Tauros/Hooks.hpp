#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "SourceEngine/SDK.hpp"

//Link with the D3D9 implementations
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace Hooks
{
    ///<summary>
    /// Sets up and hooks some functions. This should be called when the dll is loaded.
    ///</summary>
    void Initialize();

    ///<summary>
    /// Cleans up and removes the hooks. This should be called when the dll is unloaded.
    ///</summary>
    void Restore();

    ///<summary>
    /// Initialized the GUI system. Called from EndScene.
    ///</summary>
    ///<param name="pDevice"> Pointer to the D3D9 Device </param>
    void GUI_Init(IDirect3DDevice9* pDevice);

    //---------------------------------------------------
    // Hook prototypes
    //---------------------------------------------------
	using EndScene_t              = long(__stdcall*)(IDirect3DDevice9*);
	using Reset_t                 = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
    using CreateMove_t            = bool(__thiscall*)(se::IClientMode*, float, se::CUserCmd*);
    using PlaySound_t             = void(__thiscall*)(se::ISurface*, const char*);
	using OverrideMouseInput_t    = void(__thiscall*)(se::IClientMode*, float*, float*);
	using PaintTraverse_t         = void(__thiscall*)(se::IPanel*, se::VPANEL, bool, bool);
	using FrameStageNotify_t      = void(__thiscall*)(void*, se::ClientFrameStage_t);
	using OverrideView_t          = void(__thiscall*)(void*, se::CViewSetup*);
	using DrawModelExecute_t      = void(__thiscall*)(void*, se::IMatRenderContext*, const se::DrawModelState_t&, const se::ModelRenderInfo_t&, se::matrix3x4_t*);
	using SetClanTag_t            = void(__fastcall*)(const char*, const char*);
	
    //---------------------------------------------------
    // Hooked functions
    //---------------------------------------------------
    HRESULT   __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice);
    HRESULT   __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void      __stdcall Hooked_OverrideMouseInput(float* x, float* y);
    bool      __stdcall Hooked_CreateMove(float sample_input_frametime, se::CUserCmd* pCmd);
	void      __stdcall Hooked_PlaySound(const char* szFileName);
	void      __fastcall Hooked_PaintTraverse(se::IPanel* pPanel, void* edx, se::VPANEL vguiPanel, bool forceRepaint, bool allowForce);
	void      __fastcall Hooked_FrameStageNotify(void* ecx, void* edx, se::ClientFrameStage_t curStage);
	void      __fastcall Hooked_OverrideView(void* ecx, void* edx, se::CViewSetup* pViewSetup);
	void      __fastcall Hooked_DrawModelExecute(void* ecx, void* edx, se::IMatRenderContext* ctx, const se::DrawModelState_t &state, const se::ModelRenderInfo_t &pInfo, se::matrix3x4_t *pCustomBoneToWorld);

    //We're gonna replace the main window's WNDPROC for this one to capture user input and pass it down to the GUI
    LRESULT   __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}

