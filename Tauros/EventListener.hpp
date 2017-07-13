#pragma once
#include "Hacks/C4Timer.hpp"
#include "Hacks/DamageInformer.hpp"

class EventListener : public se::IGameEventListener2
{
public:
	EventListener()
	{
		se::Interfaces::EventManager()->AddListener(this, "player_hurt", false);
		se::Interfaces::EventManager()->AddListener(this, "player_spawned", false);
		se::Interfaces::EventManager()->AddListener(this, "round_start", false);
		se::Interfaces::EventManager()->AddListener(this, "bomb_planted", false);
		se::Interfaces::EventManager()->AddListener(this, "bomb_exploded", false);
	}
	~EventListener()
	{
		se::Interfaces::EventManager()->RemoveListener(this);
	}

	virtual void FireGameEvent(se::IGameEvent* event)
	{
		using namespace se;
		if (!strcmp(event->GetName(), "player_hurt"))
		{
			auto pVictim = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"))));
			auto pAttacker = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("attacker"))));
			auto health = event->GetInt("health");
			auto armor = event->GetInt("armor");
			auto weapon = event->GetString("weapon");
			auto dmg_health = event->GetInt("dmg_health");
			auto dmg_armor = event->GetInt("dmg_armor");
			auto hitgroup = event->GetInt("hitgroup"); // 1 = hs 2 = upper torso 3 = lower torso 4 = left arm 5 = right arm 6 = left leg 7 = right leg
			DamageInformer::OnPlayerHurt(pVictim, pAttacker, health, armor, weapon, dmg_health, dmg_armor, hitgroup);
		}
		else if (!strcmp(event->GetName(), "player_spawned"))
		{
			auto pLocal = C_CSPlayer::GetLocalPlayer();
			auto pSpawned = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"))));
			auto inrestart = event->GetBool("inrestart");
			if (pLocal != pSpawned)
				return;

			C4Timer::OnLocalPlayerSpawn(inrestart);
		}
		else if (!strcmp(event->GetName(), "round_start"))
		{
			auto timelimit = event->GetInt("timelimit");
			auto fraglimit = event->GetInt("fraglimit");
			auto objective = event->GetString("objective");
			C4Timer::OnRoundStart(timelimit, fraglimit, objective);
		}
		else if (!strcmp(event->GetName(), "bomb_planted"))
		{
			auto pPlanter = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"))));
			auto site = event->GetInt("site");
			C4Timer::OnBombPlanted(pPlanter, site);
		}
		else if (!strcmp(event->GetName(), "bomb_exploded"))
		{
			auto pPlanter = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"))));
			auto site = event->GetInt("site");
			C4Timer::OnBombExploded(pPlanter, site);
		}
	}

	int GetEventDebugID() override
	{
		return 42;
	}
};
