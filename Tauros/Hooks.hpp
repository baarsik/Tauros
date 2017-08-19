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
    using CreateMove_t            = void(__thiscall*)(IBaseClientDLL*, int, float, bool);
    using PlaySound_t             = void(__thiscall*)(ISurface*, const char*);
	using OverrideMouseInput_t    = void(__thiscall*)(IClientMode*, float*, float*);
	using PaintTraverse_t         = void(__thiscall*)(IPanel*, VPANEL, bool, bool);
	using FrameStageNotify_t      = void(__thiscall*)(void*, ClientFrameStage_t);
	using OverrideView_t          = void(__thiscall*)(void*, CViewSetup*);
	using DrawModelExecute_t      = void(__thiscall*)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	
    //---------------------------------------------------
    // Hooked functions
    //---------------------------------------------------
    HRESULT   __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice);
    HRESULT   __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void      __stdcall Hooked_OverrideMouseInput(float* x, float* y);
	void      __stdcall Hooked_CreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
	void      __stdcall Hooked_CreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active);
	void      __stdcall Hooked_PlaySound(const char* szFileName);
	void      __fastcall Hooked_PaintTraverse(IPanel* pPanel, void* edx, VPANEL vguiPanel, bool forceRepaint, bool allowForce);
	void      __fastcall Hooked_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage);
	void      __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup);
	void      __fastcall Hooked_DrawModelExecute(void* ecx, void* edx, IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);

    //We're gonna replace the main window's WNDPROC for this one to capture user input and pass it down to the GUI
    LRESULT   __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}

