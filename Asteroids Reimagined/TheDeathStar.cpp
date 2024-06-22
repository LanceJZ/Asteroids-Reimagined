#include "TheDeathStar.h"

TheDeathStar::TheDeathStar()
{
	for (int i = 0; i < 3; i++)
	{
		TheManagers.EM.AddEntity(FighterPairs[i] = DBG_NEW TheFighterPair());
	}
}

TheDeathStar::~TheDeathStar()
{
}

void TheDeathStar::SetWedgeModel(LineModelPoints model)
{
	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->SetWedgeModel(model);
	}

	WY = FighterPairs[0]->Fighters[0]->Radius * 0.885f;
	WX = FighterPairs[0]->Fighters[0]->Radius * 0.75f;

	float rot = 0.333333f;
	float offset = 3.75f;

	FighterPairs[0]->RotationZ = HalfPi;
	FighterPairs[1]->RotationZ = HalfPi * rot;
	FighterPairs[2]->RotationZ = HalfPi + (PI * rot);

	FighterPairs[0]->Position = { WX + offset, 0.0f, 0.0f };
	FighterPairs[1]->Position = { -WX + offset, WY, 0.0f };
	FighterPairs[2]->Position = { -WX + offset, -WY, 0.0f };
}

void TheDeathStar::SetPlayer(ThePlayer* player)
{
	Player = player;
}

bool TheDeathStar::Initialize(Utilities* utilities)
{
	Entity::Initialize(TheUtilities);

	Radius = 28.0f;

	return false;
}

bool TheDeathStar::BeginRun()
{
	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->SetParent(*this);

		for (auto &fighter : fighterPair->Fighters)
		{
			fighter->SetParent(*fighterPair);
		}
	}

	Velocity = { 20.0f, 20.0f, 0.0f };


	return false;
}

void TheDeathStar::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	CheckCollisions();

	CheckScreenEdge();
}

void TheDeathStar::Draw3D()
{
	Entity::Draw3D();

}

void TheDeathStar::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

bool TheDeathStar::CheckCollisions()
{
	for (auto &shot : Player->Shots)
	{
		if (CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			Destroy();

			return true;
		}
	}

	return false;
}

void TheDeathStar::Hit()
{
	Entity::Hit();

	for (auto &fighterPair : FighterPairs)
	{
		for (auto &fighter : fighterPair->Fighters)
		{
			auto it = std::find(fighter->Parents->begin(), fighter->Parents->end(), this);
			if (it != fighter->Parents->end()) fighter->Parents->erase(it);
		}

		fighterPair->BeforeCalculate();
		fighterPair->CalculateWorldVectors();
		fighterPair->CalculateWorldSpace();
		fighterPair->Position = fighterPair->WorldPosition;
		fighterPair->AfterCalculate();
	}

}

void TheDeathStar::Destroy()
{
	Entity::Destroy();
}
