#pragma once

class EventListener : public se::IGameEventListener2
{
public:
	EventListener()
	{
		se::Interfaces::EventManager()->AddListener(this, XorStr("player_hurt"), false);
		se::Interfaces::EventManager()->AddListener(this, XorStr("player_spawned"), false);
		se::Interfaces::EventManager()->AddListener(this, XorStr("round_start"), false);
		se::Interfaces::EventManager()->AddListener(this, XorStr("bomb_planted"), false);
		se::Interfaces::EventManager()->AddListener(this, XorStr("bomb_exploded"), false);
	}
	~EventListener()
	{
		se::Interfaces::EventManager()->RemoveListener(this);
	}

	virtual void FireGameEvent(se::IGameEvent* event)
	{
		using namespace se;
		if (!strcmp(event->GetName(), XorStr("player_hurt")))
		{
			auto pVictim = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"))));
			auto pAttacker = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("attacker"))));
			auto health = event->GetInt(XorStr("health"));
			auto armor = event->GetInt(XorStr("armor"));
			auto weapon = event->GetString(XorStr("weapon"));
			auto dmg_health = event->GetInt(XorStr("dmg_health"));
			auto dmg_armor = event->GetInt(XorStr("dmg_armor"));
			auto hitgroup = event->GetInt(XorStr("hitgroup")); // 1 = hs 2 = upper torso 3 = lower torso 4 = left arm 5 = right arm 6 = left leg 7 = right leg
			DamageInformer::OnPlayerHurt(pVictim, pAttacker, health, armor, weapon, dmg_health, dmg_armor, hitgroup);
		}
		else if (!strcmp(event->GetName(), XorStr("player_spawned")))
		{
			auto pLocal = C_CSPlayer::GetLocalPlayer();
			auto pSpawned = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"))));
			auto inrestart = event->GetBool(XorStr("inrestart"));
			if (pLocal != pSpawned)
				return;

			C4Timer::OnLocalPlayerSpawn(inrestart);
		}
		else if (!strcmp(event->GetName(), XorStr("round_start")))
		{
			auto timelimit = event->GetInt(XorStr("timelimit"));
			auto fraglimit = event->GetInt(XorStr("fraglimit"));
			auto objective = event->GetString(XorStr("objective"));
			C4Timer::OnRoundStart(timelimit, fraglimit, objective);
		}
		else if (!strcmp(event->GetName(), XorStr("bomb_planted")))
		{
			auto pPlanter = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"))));
			auto site = event->GetInt(XorStr("site"));
			C4Timer::OnBombPlanted(pPlanter, site);
		}
		else if (!strcmp(event->GetName(), XorStr("bomb_exploded")))
		{
			auto pPlanter = static_cast<C_CSPlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"))));
			auto site = event->GetInt(XorStr("site"));
			C4Timer::OnBombExploded(pPlanter, site);
		}
	}

	int GetEventDebugID() override
	{
		return 42;
	}
};
