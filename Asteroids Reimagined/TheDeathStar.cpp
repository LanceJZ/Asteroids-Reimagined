#include "TheDeathStar.h"

TheDeathStar::TheDeathStar()
{
	for (int i = 0; i < 3; i++)
	{
		EM.AddEntity(FighterPairs[i] = DBG_NEW TheFighterPair());
	}
}

TheDeathStar::~TheDeathStar()
{
}

void TheDeathStar::SetWedgeModel(std::vector<Vector3> model)
{
	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->SetWedgeModel(model);
	}
}

void TheDeathStar::SetExplodeSound(Sound sound)
{
	Enemy::SetExplodeSound(sound);

	for (const auto &fighterPair : FighterPairs)
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

	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->SetPlayer(player);
	}
}

void TheDeathStar::SetUFO()
{
	for (const auto& fighterPair : FighterPairs)
	{
		fighterPair->SetUFO(UFORefs);
	}
}

bool TheDeathStar::Initialize()
{
	Enemy::Initialize();

	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->Initialize();
	}

	Radius = 30.0f;
	Enabled = false;

	return true;
}

bool TheDeathStar::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheDeathStar::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	CheckCollisions();

}

void TheDeathStar::FixedUpdate(float deltaTime)
{
	Enemy::FixedUpdate(deltaTime);

	if (NewWave)
	{
		if (IsOffScreen())
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
	Enemy::Draw3D();

}

void TheDeathStar::NewWaveStart()
{
	NewWave = true;

	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->NewWave = true;

		for (const auto &fighter : fighterPair->Fighters)
		{
			fighter->NewWave = true;
		}
	}
}

void TheDeathStar::Reset()
{
	Enemy::Reset();

	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->Reset();

		for (const auto &fighter : fighterPair->Fighters)
		{
			fighter->Reset();
		}
	}
}

void TheDeathStar::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	if (!Player->GameOver) PlaySound(SpawnSound);

	NewWave = false;

	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->Reset();
		fighterPair->SetParent(*this);
		fighterPair->Wave = Wave;
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
	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->Reset();

		for (const auto &fighter : fighterPair->Fighters)
		{
			fighter->Reset();
		}
	}

	Reset();
}

void TheDeathStar::Hit()
{
	Enemy::Hit();

	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->Separate();

		for (const auto &fighter : fighterPair->Fighters)
		{
			fighter->RemoveParent(this);
		}
	}

	Destroy();
}

void TheDeathStar::Destroy()
{
	Enemy::Destroy();
}

bool TheDeathStar::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}