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

void TheDeathStar::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;

	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->SetExplodeSound(sound);
	}
}

void TheDeathStar::SetSpawnSound(Sound sound)
{
	SpawnSound = sound;
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

	Radius = 30.0f;
	Enabled = false;

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

	if (NewWave)
	{
		if (OffScreen())
		{
			Reset();
		}
	}
	else
	{
		CheckScreenEdge();
	}
}

void TheDeathStar::Draw3D()
{
	Entity::Draw3D();

}

void TheDeathStar::NewWaveStart()
{
	NewWave = true;

	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->NewWave = true;

		for (auto &fighter : fighterPair->Fighters)
		{
			fighter->NewWave = true;
		}
	}
}

void TheDeathStar::Reset()
{
	Enabled = false;

	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->Reset();
		fighterPair->Destroy();

		for (auto &fighter : fighterPair->Fighters)
		{
			fighter->Reset();
			fighter->Destroy();
		}
	}
}

void TheDeathStar::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	if (!Player->GameOver) PlaySound(SpawnSound);

	Velocity = { 20.0f, 20.0f, 0.0f };
	NewWave = false;

	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->Reset();
		fighterPair->SetParent(*this);
		fighterPair->Spawn(position);
	}

	WY = FighterPairs[0]->Fighters[0]->Radius * 0.85f;
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

void TheDeathStar::NewGame()
{
	for (auto &fighterPair : FighterPairs)
	{
		fighterPair->Reset();

		for (auto &fighter : fighterPair->Fighters)
		{
			fighter->Reset();
		}
	}

	Reset();
}

void TheDeathStar::CheckCollisions()
{
	if (Player->Enabled && CirclesIntersect(*Player))
	{
		Player->Hit(Position, Velocity);

		if (!Player->Shield->Enabled)
		{
			Hit();
			Destroy();
			Player->ScoreUpdate(Points);
		}

		return;
	}

	for (auto& shot : Player->Shots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			Destroy();
			Player->ScoreUpdate(Points);

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

	if (!Player->GameOver) PlaySound(ExplodeSound);

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
