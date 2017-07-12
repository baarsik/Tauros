#include "SDK.hpp"

#include "../Utils.hpp"
#include "../XorStr.hpp"

namespace se
{
    IVEngineClient*     Interfaces::m_pEngine = nullptr;
    IBaseClientDLL*     Interfaces::m_pClient = nullptr;
    IClientEntityList*  Interfaces::m_pEntityList = nullptr;
    CGlobalVarsBase*    Interfaces::m_pGlobals = nullptr;
    IPanel*             Interfaces::m_pVGuiPanel = nullptr;
    ISurface*           Interfaces::m_pVGuiSurface = nullptr;
    CInput*             Interfaces::m_pInput = nullptr;
    IEngineTrace*       Interfaces::m_pEngineTrace = nullptr;
    ICvar*              Interfaces::m_pCVar = nullptr;
    IClientMode*        Interfaces::m_pClientMode = nullptr;
	IVModelInfo*        Interfaces::m_pModelInfo = nullptr;
	IVRenderView*       Interfaces::m_pRenderView = nullptr;
	IVModelRender*      Interfaces::m_pModelRender = nullptr;
	IMaterialSystem*    Interfaces::m_pMaterialSystem = nullptr;
	IGameEventManager2* Interfaces::m_pGameEventManager2 = nullptr;

    CreateInterfaceFn GetFactory(HMODULE hMod)
    {
        return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(hMod, XorStr("CreateInterface")));
    }

    template<typename T>
    T* CaptureInterface(CreateInterfaceFn f, const char* szInterfaceVersion)
    {
        return reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));
    }

    IVEngineClient* Interfaces::Engine()
    {
        if(!m_pEngine)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
            m_pEngine = CaptureInterface<IVEngineClient>(pfnFactory, XorStr("VEngineClient014"));
        }
        return m_pEngine;
    }
    IBaseClientDLL* Interfaces::Client()
    {
        if(!m_pClient)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("client.dll")));
            m_pClient = CaptureInterface<IBaseClientDLL>(pfnFactory, XorStr("VClient018"));
        }
        return m_pClient;
    }
    IClientEntityList* Interfaces::EntityList()
    {
        if(!m_pEntityList)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("client.dll")));
            m_pEntityList = CaptureInterface<IClientEntityList>(pfnFactory, XorStr("VClientEntityList003"));
        }
        return m_pEntityList;
    }
    CGlobalVarsBase* Interfaces::GlobalVars()
    {
        if(!m_pGlobals)
		{
			auto pClientVFTable = *reinterpret_cast<uint32_t**>(Client());
			m_pGlobals = **reinterpret_cast<CGlobalVarsBase***>(pClientVFTable[0] + 0x1B);
        }
        return m_pGlobals;
    }
    IPanel* Interfaces::VGUIPanel()
    {
        if(!m_pVGuiPanel)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("vgui2.dll")));
            m_pVGuiPanel = CaptureInterface<IPanel>(pfnFactory, XorStr("VGUI_Panel009"));
        }
        return m_pVGuiPanel;
    }
    ISurface* Interfaces::MatSurface()
    {
        if(!m_pVGuiSurface)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("vguimatsurface.dll")));
            m_pVGuiSurface = CaptureInterface<ISurface>(pfnFactory, XorStr("VGUI_Surface031"));
        }
        return m_pVGuiSurface;
    }
    CInput* Interfaces::Input()
    {
        if(!m_pInput)
		{
            auto pClientVFTable = *reinterpret_cast<uint32_t**>(Client());
            m_pInput = *reinterpret_cast<CInput**>(pClientVFTable[15] + 0x1);
        }
        return m_pInput;
    }
	IGameEventManager2* Interfaces::EventManager()
    {
        if(!m_pGameEventManager2)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pGameEventManager2 = CaptureInterface<IGameEventManager2>(pfnFactory, XorStr("GAMEEVENTSMANAGER002"));
        }
        return m_pGameEventManager2;
    }
	IEngineTrace* Interfaces::EngineTrace()
	{
		if (!m_pEngineTrace)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pEngineTrace = CaptureInterface<IEngineTrace>(pfnFactory, XorStr("EngineTraceClient004"));
		}
		return m_pEngineTrace;
	}
    ICvar* Interfaces::CVar()
    {
        if(!m_pCVar)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("vstdlib.dll")));
            m_pCVar = CaptureInterface<ICvar>(pfnFactory, XorStr("VEngineCvar007"));
        }
        return m_pCVar;
    }
    IClientMode* Interfaces::ClientMode()
    {
        if(!m_pClientMode)
		{
			auto pClientVFTable = *reinterpret_cast<uint32_t**>(Client());
			m_pClientMode = **reinterpret_cast<IClientMode***>(pClientVFTable[10] + 0x5);
        }
        return m_pClientMode;
    }
	IVModelInfo* Interfaces::ModelInfo()
    {
	    if (!m_pModelInfo)
	    {
		    auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pModelInfo = CaptureInterface<IVModelInfo>(pfnFactory, XorStr("VModelInfoClient004"));
	    }
		return m_pModelInfo;
    }
	IVRenderView* Interfaces::RenderView()
	{
		if (!m_pRenderView)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pRenderView = CaptureInterface<IVRenderView>(pfnFactory, XorStr("VEngineRenderView014"));
		}
		return m_pRenderView;
	}
	IVModelRender* Interfaces::ModelRender()
    {
		if (!m_pModelRender)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pModelRender = CaptureInterface<IVModelRender>(pfnFactory, XorStr("VEngineModel016"));
		}
		return m_pModelRender;
    }
	IMaterialSystem* Interfaces::MaterialSystem()
    {
		if (!m_pMaterialSystem)
		{
			auto pfnFactory = GetFactory(GetModuleHandleA(XorStr("materialsystem.dll")));
			m_pMaterialSystem = CaptureInterface<IMaterialSystem>(pfnFactory, XorStr("VMaterialSystem080"));
		}
		return m_pMaterialSystem;
    }
}