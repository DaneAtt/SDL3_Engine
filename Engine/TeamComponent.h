#pragma once
#include "ECS.h"

class TeamComponent : public Component
{
public:

	enum Behavior
	{
		Aggressive,
		Peaceful,
		Neutral
	};

	enum entityTeam
	{
		playerTeam,
		enemyTeam,
		neutralTeam
	};

	struct Team
	{
		Behavior behavior;
		entityTeam EntityTeam;
	};

	TeamComponent(Team team) : eTeam(team) {};
	~TeamComponent() {};

	bool isHostileTo(TeamComponent& other)
	{
		if (eTeam.behavior == Behavior::Peaceful || eTeam.behavior == Behavior::Neutral)
		{
			return false;
		}
		else if (eTeam.behavior == Behavior::Aggressive)
		{
			if (eTeam.EntityTeam != other.eTeam.EntityTeam)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	void setTeam(entityTeam newTeam)
	{
		eTeam.EntityTeam = newTeam;
	}

	void setBehavior(Behavior newBehavior)
	{
		eTeam.behavior = newBehavior;
	}

	entityTeam getTeam() { return eTeam.EntityTeam; }


private:
	Team eTeam;

};