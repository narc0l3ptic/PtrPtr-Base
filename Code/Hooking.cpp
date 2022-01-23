#include "Hooking.hpp"
#include "Game.hpp"
#include "ScriptManager.hpp"
#include "CustomText.hpp"
#include "Util.hpp"
#include <MinHook/MinHook.h>
#include "HackFuncs.h"

#ifdef BIGBASE_DEBUG
#  pragma comment(lib, "MinHook-Debug.lib")
#else
#  pragma comment(lib, "MinHook-Release.lib")
#endif

namespace Big
{
	namespace
	{
		std::uint32_t g_HookFrameCount{};
	}

	bool Hooks::IsDlcPresent(std::uint32_t hash)
	{
		if (g_Running && g_HookFrameCount != *g_GameVariables->m_FrameCount)
		{
			g_HookFrameCount = *g_GameVariables->m_FrameCount;
			g_ScriptManager->OnGameTick();
		}

		return static_cast<decltype(&IsDlcPresent)>(g_Hooking->m_OriginalIsDlcPresent)(hash);
	}

	const char* Hooks::GetLabelText(void* unk, const char* label)
	{
		if (g_Running)
			if (auto text = g_CustomText->GetText(Joaat(label)))
				return text;

		return static_cast<decltype(&GetLabelText)>(g_Hooking->m_OriginalGetLabelText)(unk, label);
	}

	bool Hooks::GetEventData(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount)
	{
		auto result = static_cast<decltype(&GetEventData)>(g_Hooking->m_OriginalGetEventData)(eventGroup, eventIndex, args, argCount);
		
		if (result && g_LogScriptEvents)
		{
			g_Logger->Info("Script event group: %i", eventGroup);
			g_Logger->Info("Script event index: %i", eventIndex);
			g_Logger->Info("Script event argcount: %i", argCount);
			for (std::uint32_t i = 0; i < argCount; ++i)
				g_Logger->Info("Script event args[%u] : %" PRIi64, i, args[i]);
		}

		return result;
	}

	CBaseModelInfo* Hooks::GetModelInfo(uint32_t model, DWORD* unk)
	{
		if (lesterCasino)
		{
			switch (model)
			{
			case Joaat("CSB_Agatha"):
			case Joaat("IG_Agatha"):
			case Joaat("CSB_Avery"):
			case Joaat("IG_Avery"):
			case Joaat("CSB_Brucie2"):
			case Joaat("IG_Brucie2"):
			case Joaat("IG_TaoCheng2"):
			case Joaat("CS_TaoCheng2"):
			case Joaat("IG_TaosTranslator2"):
			case Joaat("CS_TaosTranslator2"):
			case Joaat("CSB_Thornton"):
			case Joaat("IG_Thornton"):
			case Joaat("CSB_TomCasino"):
			case Joaat("IG_TomCasino"):
			case Joaat("CSB_Vincent"):
			case Joaat("IG_Vincent"):
			case Joaat("A_F_Y_GenCasPat_01"):
			case Joaat("A_F_Y_SmartCasPat_01"):
			case Joaat("A_M_M_MLCrisis_01"):
			case Joaat("A_M_Y_GenCasPat_01"):
			case Joaat("A_M_Y_SmartCasPat_01"):
			case Joaat("G_M_M_CasRN_01"):
			case Joaat("S_M_Y_WestSec_01"):
			case Joaat("S_F_Y_Casino_01"):
			case Joaat("S_M_Y_Casino_01"):
			case Joaat("U_F_O_Carol"):
			case Joaat("U_F_O_Eileen"):
			case Joaat("U_F_M_CasinoCash_01"):
			case Joaat("U_F_M_CasinoShop_01"):
			case Joaat("U_F_M_Debbie_01"):
			case Joaat("U_F_Y_Beth"):
			case Joaat("U_F_Y_Lauren"):
			case Joaat("U_F_Y_Taylor"):
			case Joaat("U_M_M_Blane"):
			case Joaat("U_M_M_Curtis"):
			case Joaat("U_M_M_Vince"):
			case Joaat("U_M_O_Dean"):
			case Joaat("U_M_Y_Caleb"):
			case Joaat("U_M_Y_CroupThief_01"):
			case Joaat("U_M_Y_Gabriel"):
			case Joaat("U_M_Y_Ushi"):
				model = Joaat("a_c_chimp");
			}
		}

		return static_cast<decltype(&GetModelInfo)>(g_Hooking->m_OriginalGetModelInfo)(model, unk);
	}

	Hooking::Hooking():
		m_D3DHook(g_GameVariables->m_Swapchain, 18),
		m_ObjectBypass(Signature("74 48 E8 ? ? ? ? 48 8B 48 48").Scan().Add(3).Rip().Add(30).As<char*>())
	{
		std::copy_n(m_ObjectBypass, 2, m_OriginalObjectBypass);

		MH_Initialize();
		MH_CreateHook(g_GameFunctions->m_IsDlcPresent, &Hooks::IsDlcPresent, &m_OriginalIsDlcPresent);
		MH_CreateHook(g_GameFunctions->m_GetLabelText, &Hooks::GetLabelText, &m_OriginalGetLabelText);
		MH_CreateHook(g_GameFunctions->m_GetEventData, &Hooks::GetEventData, &m_OriginalGetEventData);
		MH_CreateHook(g_GameFunctions->m_GetModelInfo, &Hooks::GetModelInfo, &m_OriginalGetModelInfo);
	}

	Hooking::~Hooking() noexcept
	{
		MH_RemoveHook(g_GameFunctions->m_WndProc);
		MH_RemoveHook(g_GameFunctions->m_GetEventData);
		MH_RemoveHook(g_GameFunctions->m_GetLabelText);
		MH_RemoveHook(g_GameFunctions->m_IsDlcPresent);
		MH_RemoveHook(g_GameFunctions->m_GetModelInfo);
		MH_Uninitialize();
	}

	void Hooking::Hook()
	{
		std::fill_n(m_ObjectBypass, 2, '\x90');
		m_D3DHook.Enable();
		MH_EnableHook(MH_ALL_HOOKS);
	}

	void Hooking::Unhook()
	{
		std::copy_n(m_OriginalObjectBypass, 2, m_ObjectBypass);
		m_D3DHook.Disable();
		MH_DisableHook(MH_ALL_HOOKS);
	}
}
