#pragma once
#include "Classes.h"

namespace rage
{
	struct scrNativeRegistration;
}

namespace Big
{
	class GameVariables
	{
	public:
		explicit GameVariables();
		~GameVariables() noexcept = default;
		GameVariables(GameVariables const&) = delete;
		GameVariables(GameVariables&&) = delete;
		GameVariables& operator=(GameVariables const&) = delete;
		GameVariables& operator=(GameVariables&&) = delete;

		void PostInit();

		std::uint32_t* m_GameState;
		std::uint32_t* m_FrameCount;
		HWND m_GameWindow;
		IDXGISwapChain* m_Swapchain;
		rage::scrNativeRegistration** m_NativeRegistrations;
		std::uint64_t** m_GlobalBase;
		char* m_GameBuild;
		PVOID m_ModelSpawnBypass;
	};

	class GameFunctions
	{
	public:
		explicit GameFunctions();
		~GameFunctions() noexcept = default;
		GameFunctions(GameFunctions const&) = delete;
		GameFunctions(GameFunctions&&) = delete;
		GameFunctions& operator=(GameFunctions const&) = delete;
		GameFunctions& operator=(GameFunctions&&) = delete;

		using IsDlcPresent = bool(std::uint32_t hash);
		IsDlcPresent* m_IsDlcPresent;

		using WndProc = LRESULT(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		WndProc* m_WndProc;

		using GetEventData = bool(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount);
		GetEventData* m_GetEventData;

		using GetLabelText = const char*(void* unk, const char* label);
		GetLabelText* m_GetLabelText;

		using TriggerScriptEvent = void(int eventGroup, std::uint64_t* argstruct, int argCount, int bitflag);
		TriggerScriptEvent* m_TriggerScriptEvent;

		using get_model_info = CBaseModelInfo * (*)(uint32_t Hash, DWORD* unk);
		get_model_info* m_GetModelInfo;
	};

	inline std::unique_ptr<GameVariables> g_GameVariables;
	inline std::unique_ptr<GameFunctions> g_GameFunctions;
}
