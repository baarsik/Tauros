#pragma once
#include "Hacks/DamageIndicator.hpp"

class EventListener : public se::IGameEventListener2
{
public:
	EventListener()
	{
		se::Interfaces::EventManager()->AddListener(this, "player_hurt", false);
	}
	~EventListener()
	{
		se::Interfaces::EventManager()->RemoveListener(this);
	}

	virtual void FireGameEvent(se::IGameEvent* event)
	{
		if (!strcmp(event->GetName(), "player_hurt"))
		{
			DamageIndicator::FireEvent_Post(event);
		}
	}

	int GetEventDebugID() override
	{
		return 42;
	}
};
