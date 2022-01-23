#pragma once
#include "ScriptCallback.hpp"
#include "Natives.hpp"
#include "Game.hpp"
#include "Common.hpp"

namespace Big
{
	inline bool spawnInVehicle = false;
	inline bool lesterCasino = false;

	inline void GodModeBooleanFunction(bool onoff)
	{
		if (onoff) { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 500); }
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), onoff);
	}

	inline void NeverWantedBooleanFunction(bool onoff)
	{
		if (onoff) { PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, 0); }
		PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), onoff);
	}

	inline void LocalInvisibilityBooleanFunction(bool onoff)
	{
		if (onoff)
		{
			ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 0, 0);
		}
		else
		{
			ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 255, 0);
		}
	}

	inline void SuperJumpBooleanFunction(bool onoff)
	{
		if (onoff)
		{
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_PED_ID());
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
		}
	}

	inline void InfiniteAmmoBooleanFunction(bool onoff)
	{
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), onoff);
	}

	inline float degToRad(float degs)
	{
		return degs * 3.141592653589793f / 180.f;
	}

	inline void SpawnVehicle(Hash hash, NativeVector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true))
	{
		if (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			STREAMING::REQUEST_MODEL(hash);
			Sleep(0);
		}

		float forward = 10.f;
		float heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		float xVector = forward * sin(degToRad(heading)) * -1.f;
		float yVector = forward * cos(degToRad(heading));

		g_CallbackScript->AddCallback<ModelCallback>((hash), [=] {
			*(unsigned short*)g_GameVariables->m_ModelSpawnBypass = 0x9090;
			Vehicle vehicle = VEHICLE::CREATE_VEHICLE(hash, coords.x + xVector, coords.y + yVector, coords.z, heading, true, false, false);
			*(unsigned short*)g_GameVariables->m_ModelSpawnBypass = 0x0574;

			ENTITY::_SET_ENTITY_SOMETHING(vehicle, true);
			DECORATOR::DECOR_SET_INT(vehicle, "MPBitset", 0);
			ENTITY::SET_ENTITY_MAX_SPEED(vehicle, 25000);

			if (spawnInVehicle)
			{
				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), vehicle, -1);
			}
		});
	}

	inline void AutoFixVehicle(bool onoff)
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		if (onoff && ENTITY::GET_ENTITY_HEALTH(veh) < 1000)
		{
			for (int i{}; i < 7; ++i)
			{
				VEHICLE::FIX_VEHICLE_WINDOW(veh, i);
				VEHICLE::SET_VEHICLE_TYRE_FIXED(veh, i);
			}

			VEHICLE::SET_VEHICLE_FIXED(veh);
		}
	}

	inline void SuperRun(bool onoff)
	{
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_PED_ID(), (onoff && GetKeyState(VK_SHIFT) & 0x8000) ? 1.49 : 1);
	}
}