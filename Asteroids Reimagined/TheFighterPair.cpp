#include "TheFighterPair.h"

TheFighterPair::TheFighterPair()
{
	for (int i = 0; i < 2; i++)
	{
		TheManagers.EM.AddLineModel(Fighters[i] = DBG_NEW TheFighter());
	}
}

TheFighterPair::~TheFighterPair()
{
}

void TheFighterPair::SetPlayer(ThePlayer* player)
{
	Player = player;

	for (auto &fighter : Fighters)
	{
		fighter->SetPlayer(player);
	}
}

void TheFighterPair::SetWedgeModel(LineModelPoints model)
{
	for (auto &fighter : Fighters)
	{
		fighter->SetModel(model);
	}

	Fighters[0]->Position.x = Fighters[0]->Radius * 0.875f;
	Fighters[1]->Position.x = -Fighters[1]->Radius * 0.875f;
}

bool TheFighterPair::Initialize(Utilities* utilities)
{
	Entity::Initialize(TheUtilities);

	for (auto &fighterPair : Fighters)
	{
		fighterPair->Initialize(TheUtilities);
	}

	return false;
}

bool TheFighterPair::BeginRun()
{
	Fighters[1]->RotationZ = PI;

	return false;
}

void TheFighterPair::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (Separated)
	{
		ChasePlayer();

		CheckScreenEdge();
	}

	CheckCollisions();
}

void TheFighterPair::Draw3D()
{
	Entity::Draw3D();

}

void TheFighterPair::Separate()
{
	Separated = true;
	Position = GetWorldPosition();
	Radius = 16.0f;

	IsChild = false;
}

void TheFighterPair::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void TheFighterPair::Hit()
{
	Entity::Hit();

	for (auto &fighter : Fighters)
	{
		fighter->Separate();

		auto it = std::find(fighter->Parents->begin(), fighter->Parents->end(), this);
		if (it != fighter->Parents->end()) fighter->Parents->erase(it);

	}
}

void TheFighterPair::Destroy()
{
	Entity::Destroy();
}

void TheFighterPair::ChasePlayer()
{
	SetRotateVelocity(Player->Position, TurnSpeed, Speed);
}

void TheFighterPair::CheckCollisions()
{
	//auto it = std::find(fighter->Parents->begin(), fighter->Parents->end(), this);
	//if (it != fighter->Parents->end()) fighter->Parents->erase(it);

	for (auto shot : Player->Shots)
	{
		if (CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			Destroy();
		}
	}
}
