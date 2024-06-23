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
}

void TheDeathStar::SetPlayer(ThePlayer* player)
{
	Player = player;

	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->SetPlayer(player);
	}
}

void TheDeathStar::SetUFO(TheUFO* ufo[2])
{
	for (int i = 0; i < 2; i++)
	{
		UFOs[i] = ufo[i];
	}

	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->SetUFO(ufo);
	}
}

bool TheDeathStar::Initialize(Utilities* utilities)
{
	Entity::Initialize(TheUtilities);

	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->Initialize(TheUtilities);
	}

	Radius = 28.0f;

	return true;
}

bool TheDeathStar::BeginRun()
{

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

	Velocity = { 20.0f, 20.0f, 0.0f };

	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->SetParent(*this);
		fighterPair->Spawn(position);
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

void TheDeathStar::CheckCollisions()
{
	if (Player->Enabled && CirclesIntersect(*Player))
	{
		Hit();
		Player->Hit();
		Destroy();
		return;
	}

	for (auto& shot : Player->Shots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			Destroy();
			return;
		}
	}

	for (auto& ufo : UFOs)
	{
		if (ufo->Enabled && CirclesIntersect(*ufo))
		{
			ufo->Hit();
			ufo->Destroy();
			Hit();
			Destroy();
			return;
		}

		for (auto& shot : ufo->Shots)
		{
			if (shot->Enabled && CirclesIntersect(*shot))
			{
				shot->Destroy();
				ufo->Hit();
				ufo->Destroy();
				Hit();
				Destroy();
				return;
			}
		}
	}
}

void TheDeathStar::Hit()
{
	Entity::Hit();

	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->Separate();

		for (auto &fighter : fighterPair->Fighters)
		{
			fighter->RemoveParent(this);
		}
	}

}

void TheDeathStar::Destroy()
{
	Entity::Destroy();
}
