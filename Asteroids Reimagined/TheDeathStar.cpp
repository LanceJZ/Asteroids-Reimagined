#include "TheDeathStar.h"

TheDeathStar::TheDeathStar()
{
	for (int i = 0; i < 3; i++)
	{
		Managers.EM.AddEntity(FighterPairs[i] = DBG_NEW TheFighterPair());
	}
}

TheDeathStar::~TheDeathStar()
{
}

void TheDeathStar::SetWedgeModel(LineModelPoints model)
{
	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->SetWedgeModel(model);
	}
}

void TheDeathStar::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;

	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->SetExplodeSound(sound);
	}
}

void TheDeathStar::SetSpawnSound(Sound sound)
{
	SpawnSound = sound;
}

void TheDeathStar::SetParticleManager(ParticleManager* particleManager)
{
	Particles = particleManager;

	for (const auto& fighterPair : FighterPairs)
	{
		fighterPair->SetParticleManager(particleManager);

		for (const auto& fighter : fighterPair->Fighters)
		{
			fighter->SetParticleManager(particleManager);
		}
	}
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

void TheDeathStar::SetEnemies(Enemy* enemyOne, Enemy* enemyTwo)
{
	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->SetEnemies(enemyOne, enemyTwo);
	}
}

bool TheDeathStar::Initialize(Utilities* utilities)
{
	Entity::Initialize(TheUtilities);

	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->Initialize(TheUtilities);
	}

	Radius = 30.0f;
	Enabled = false;

	return true;
}

bool TheDeathStar::BeginRun()
{
	Entity::BeginRun();

	return false;
}

void TheDeathStar::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

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
	//Enabled = false;

	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->Reset();
		//fighterPair->Destroy();

		for (const auto &fighter : fighterPair->Fighters)
		{
			fighter->Reset();
			//fighter->Destroy();
		}
	}
}

void TheDeathStar::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	if (!Player->GameOver) PlaySound(SpawnSound);

	Velocity = { 20.0f, 20.0f, 0.0f };
	NewWave = false;

	for (const auto &fighterPair : FighterPairs)
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
	Entity::Destroy();

	if (!Player->GameOver) PlaySound(ExplodeSound);

	for (const auto &fighterPair : FighterPairs)
	{
		fighterPair->Separate();

		for (const auto &fighter : fighterPair->Fighters)
		{
			fighter->RemoveParent(this);
		}
	}

}

void TheDeathStar::Destroy()
{
	Entity::Destroy();
}

bool TheDeathStar::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;

	//if (Player->Enabled && CirclesIntersect(*Player))
	//{
	//	Player->Hit(Position, Velocity);

	//	if (!Player->Shield->Enabled)
	//	{
	//		Hit();
	//		Player->ScoreUpdate(Points);
	//	}

	//	return;
	//}

	//for (const auto& shot : Player->Shots)
	//{
	//	if (shot->Enabled && CirclesIntersect(*shot))
	//	{
	//		shot->Destroy();
	//		Hit();
	//		Player->ScoreUpdate(Points);

	//		return;
	//	}
	//}

	//for (const auto& shot : Player->DoubleShots)
	//{
	//	if (shot->Enabled && CirclesIntersect(*shot))
	//	{
	//		shot->Destroy();
	//		Hit();
	//		Player->ScoreUpdate(Points);

	//		return;
	//	}
	//}

	//for (const auto& bigShot : Player->BigShots)
	//{
	//	if (bigShot->Enabled && CirclesIntersect(*bigShot))
	//	{
	//		bigShot->Destroy();
	//		Hit();
	//		Player->ScoreUpdate(Points);

	//		return;
	//	}
	//}

	//for (const auto& ufo : UFOs)
	//{
	//	if (ufo->Enabled && CirclesIntersect(*ufo))
	//	{
	//		ufo->Hit();
	//		Hit();
	//		return;
	//	}

	//	for (const auto& shot : ufo->Shots)
	//	{
	//		if (shot->Enabled && CirclesIntersect(*shot))
	//		{
	//			shot->Destroy();
	//			Hit();
	//			return;
	//		}
	//	}
	//}
}