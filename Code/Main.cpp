#include "Game.hpp"
#include "Hooking.hpp"
#include "ScriptManager.hpp"
#include "MainScript.hpp"
#include "ScriptCallback.hpp"
#include "Invoker.hpp"
#include "CustomText.hpp"
#include "UIManagement.h"

/**
 * \brief The entry point of the library
 * \param hInstance The base address of the DLL
 * \param reason The reason for this function being called
 */
BOOL DllMain(HINSTANCE hInstance, DWORD reason, LPVOID)
{
	using namespace Big;

	if (reason == DLL_PROCESS_ATTACH)
	{
		g_Module = hInstance;
		CreateThread(nullptr, 0, [](LPVOID) -> DWORD
		{
			g_Logger = std::make_unique<Logger>();
			g_Logger->Info("Pointer injected.");

			g_GameFunctions = std::make_unique<GameFunctions>();
			g_GameVariables = std::make_unique<GameVariables>();

			// Wait for the game to load
			while (*g_GameVariables->m_GameState != 0)
			{
				std::this_thread::sleep_for(3s);
				std::this_thread::yield();
			}

			g_GameVariables->PostInit();

			g_Invoker = std::make_unique<Invoker>();
			g_CustomText = std::make_unique<CustomText>();
			g_UIManagement = std::make_unique<UIManagement>();

			g_ScriptManager = std::make_unique<ScriptManager>();
			g_MainScript = std::make_shared<MainScript>();
			g_CallbackScript = std::make_shared<CallbackScript>();
			g_ScriptManager->AddScript(g_MainScript);
			g_ScriptManager->AddScript(g_CallbackScript);

			g_Hooking = std::make_unique<Hooking>();
			g_Hooking->Hook();

			g_Logger->Info("Pointer loaded.");
			while (g_Running)
			{
				if (IsKeyPressed(VK_DELETE))
					g_Running = false;

				std::this_thread::sleep_for(3ms);
				std::this_thread::yield();
			}

			std::this_thread::sleep_for(500ms);
			g_Hooking->Unhook();

			g_ScriptManager.reset();
			g_MainScript.reset();
			g_CallbackScript.reset();

			g_CustomText->RemoveText(CONSTEXPR_JOAAT("HUD_JOINING"));
			g_CustomText->RemoveText(CONSTEXPR_JOAAT("HUD_TRANSP"));

			g_CustomText.reset();
			g_Invoker.reset();

			std::this_thread::sleep_for(500ms);

			g_Hooking.reset();

			g_GameVariables.reset();
			g_GameFunctions.reset();
			
			g_Logger->Info("Pointer unloaded.");
			g_Logger.reset();

			FreeLibraryAndExitThread(g_Module, 0);
		}, nullptr, 0, nullptr);
	}

	return true;
}
