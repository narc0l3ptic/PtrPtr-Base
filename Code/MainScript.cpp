#include "MainScript.hpp"
#include "ScriptCallback.hpp"
#include "Natives.hpp"
#include "Timer.hpp"
#include "CustomText.hpp"
#include "UIManagement.h"
#include "RegularOption.h"
#include "SubOption.h"
#include "BoolOption.h"
#include "HackFuncs.h"

namespace Big
{
	bool godModeBoolean = false;
	bool neverWantedBoolean = false;
	bool localInvisibilityBoolean = false;
	bool superJumpBoolean = false;
	bool infiniteAmmoBoolean = false;
	bool autoFixVehicleBoolean = false;
	bool superRunBoolean = false;

	bool MainScript::IsInitialized()
	{
		return m_Initialized;
	}

	ScriptType MainScript::GetType()
	{
		return ScriptType::Game;
	}

	void SelfSub()
	{
		int selfIndex = g_UIManagement->add_parent_sub("Self");
		int selfHomeIndex = g_UIManagement->m_ChildParentSubs[selfIndex].add_child_sub("Home");

		ParentSub* parentSub = &g_UIManagement->m_ChildParentSubs[selfIndex];

		static BoolOption godModeBooleanOption{ "God mode", godModeBoolean };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(godModeBooleanOption);

		static RegularOption suicideRegularOption{ "Easy way out", [] { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0); } };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(suicideRegularOption);

		static BoolOption neverWantedBooleanOption{ "Never wanted", neverWantedBoolean };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(neverWantedBooleanOption);

		static RegularOption removeCopsFromAreaOption{ "Remove police from area", [] {
			auto playerCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			MISC::CLEAR_AREA_OF_COPS(playerCoords.x, playerCoords.y, playerCoords.z, 15000, 0);
		} };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(removeCopsFromAreaOption);

		static BoolOption localInvisibilityBooleanOption{ "Invisibility", localInvisibilityBoolean };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(localInvisibilityBooleanOption);

		static BoolOption superJumpBooleanOption{ "Super jump", superJumpBoolean };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(superJumpBooleanOption);

		int selfWeaponsIndex = parentSub->add_child_sub("Weapons");

		static SubOption selfWeaponsSubOption{ "Weapon options", selfIndex, selfWeaponsIndex };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(selfWeaponsSubOption);

		static RegularOption selfWeaponsEmergencyWeaponOption{ "Get emergency gun", [] { WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Joaat("weapon_rayminigun"), 1500, false, true); } };
		parentSub->m_ChildSubs[selfWeaponsIndex].add_option(selfWeaponsEmergencyWeaponOption);

		static BoolOption selfWeaponsInfiniteAmmoBooleanOption{ "Infinite ammo", infiniteAmmoBoolean };
		parentSub->m_ChildSubs[selfWeaponsIndex].add_option(selfWeaponsInfiniteAmmoBooleanOption);

		int selfTeleportationIndex = parentSub->add_child_sub("Teleportation");

		static SubOption selfTeleportationSubOption{ "Teleportation", selfIndex, selfTeleportationIndex };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(selfTeleportationSubOption);

		int selfMiscIndex = parentSub->add_child_sub("Misc");
		
		static SubOption selfMiscSubOption{ "Misc", selfIndex, selfMiscIndex };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(selfMiscSubOption);

		static BoolOption casinoSwapBooleanOption{ "Casino model swap", lesterCasino };
		parentSub->m_ChildSubs[selfMiscIndex].add_option(casinoSwapBooleanOption);
	}

	void SpawnerSub()
	{
		int spawnerIndex = g_UIManagement->add_parent_sub("Spawn");
		int spawnerHomeIndex = g_UIManagement->m_ChildParentSubs[spawnerIndex].add_child_sub("Home");

		ParentSub* parentSub = &g_UIManagement->m_ChildParentSubs[spawnerIndex];

		int spawnerOptionsIndex = g_UIManagement->m_ChildParentSubs[spawnerIndex].add_child_sub("Spawner options");

		static SubOption spawnerOptionsSubOption{ "Spawner options", spawnerIndex, spawnerOptionsIndex };
		parentSub->m_ChildSubs[spawnerHomeIndex].add_option(spawnerOptionsSubOption);

		static BoolOption spawnerOptionsSpawnInVehicleBooleanOption{ "Spawn in vehicle", spawnInVehicle };
		parentSub->m_ChildSubs[spawnerOptionsIndex].add_option(spawnerOptionsSpawnInVehicleBooleanOption);

		static RegularOption spawnTankOption{ "Mini Cooper", [] { SpawnVehicle(Joaat("khanjali")); } };
		parentSub->m_ChildSubs[spawnerHomeIndex].add_option(spawnTankOption);

		static RegularOption spawnDeluxoOption{ "Deluxo", [] { SpawnVehicle(Joaat("deluxo")); } };
		parentSub->m_ChildSubs[spawnerHomeIndex].add_option(spawnDeluxoOption);
	}

	void PlayerSub()
	{
		// This variable is used for the initialising of the subs we only want to make once.
		static bool initList = false;

		// This is our playerlist of SubOptions.
		static std::vector<SubOption> playerlist{};

		// Here we are doing our standard create-and-store for our child and parent subs.
		static int playerSub = g_UIManagement->add_parent_sub("Players");
		static int playerHomeIndex = g_UIManagement->m_ChildParentSubs[playerSub].add_child_sub("Home");
		static int playerListIndex = g_UIManagement->m_ChildParentSubs[playerSub].add_child_sub("Player list");
		static int playerSelectedIndex = g_UIManagement->m_ChildParentSubs[playerSub].add_child_sub("Selected player");
	
		// This is just a pointer to our playerlist child sub to make later coding easier/simpler.
		static Sub* player_list_sub = &g_UIManagement->m_ChildParentSubs[playerSub].m_ChildSubs[playerListIndex];

		// On each tick we clear our old vectors (options) and make new ones to stay up-to-date.
		player_list_sub->m_Options.clear();
		playerlist.clear();

		// We loop through the players and store valid players to our playerlist vector, and we also set the selectedplayer in each one.
		for (int i{}; i < 32; i++)
		{
			if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
			{

				playerlist.push_back(SubOption{ PLAYER::GET_PLAYER_NAME(i), playerSub, playerSelectedIndex, [=]
				{
					g_SelectedPlayer = i;
				}});
			}
		}

		// Now we re-add the options to the option vector of the playerlist submenu.
		for (int i{}; i < playerlist.size(); i++)
			player_list_sub->add_option(playerlist[i]);

		// Now we can actually make our player options.
		// I will make a CEO kick to demonstrate.
		static RegularOption CEOKickOption{ "CEO kick player", [=] {
			uint64_t ceo_kick_1[4]{ 248967238, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1, 5 };
			g_GameFunctions->m_TriggerScriptEvent(1, ceo_kick_1, 4, 1 << g_SelectedPlayer);
		}};

		// We only need to show one SubOption: the playerlist sub.
		// We only want to add it once, otherwise the menu crashes.
		// We add our player options in here too.
		static SubOption playerListSubOption{ "Player list", playerSub, playerListIndex };
		if (!initList)
		{
			g_UIManagement->m_ChildParentSubs[playerSub].m_ChildSubs[playerHomeIndex].add_option(playerListSubOption);
			g_UIManagement->m_ChildParentSubs[playerSub].m_ChildSubs[playerSelectedIndex].add_option(CEOKickOption);
			initList = true;
		}
	}

	void MainScript::Initialize()
	{
		m_Initialized = true;

		g_CustomText->AddText(CONSTEXPR_JOAAT("HUD_JOINING"), "OOPSIE WOOPSIE!! Uwu We made a fucky wucky!! A wittle fucko boingo! The code monkeys at our headquarters are working VEWY HAWD to fix this!");
		g_CustomText->AddText(CONSTEXPR_JOAAT("HUD_TRANSP"), "OOPSIE WOOPSIE!! Uwu We made a fucky wucky!! A wittle fucko boingo! The code monkeys at our headquarters are working VEWY HAWD to fix this!");

		SelfSub();
		SpawnerSub();
	}

	void MainScript::Destroy()
	{
		GodModeBooleanFunction(false);
		NeverWantedBooleanFunction(false);
		LocalInvisibilityBooleanFunction(false);
		SuperJumpBooleanFunction(false);
		InfiniteAmmoBooleanFunction(false);
		AutoFixVehicle(false);
	}

	void MainScript::Tick()
	{
		// PLAYER TICKER
		PlayerSub();

		g_UIManagement->tick();
		GodModeBooleanFunction(godModeBoolean);
		NeverWantedBooleanFunction(neverWantedBoolean);
		LocalInvisibilityBooleanFunction(localInvisibilityBoolean);
		SuperJumpBooleanFunction(superJumpBoolean);
		InfiniteAmmoBooleanFunction(infiniteAmmoBoolean);
		AutoFixVehicle(autoFixVehicleBoolean);
		SuperRun(superRunBoolean);
	}
}
