#include "Enemy2.h"

Enemy2::Enemy2()
{
	LayMineTimerID = Managers.EM.AddTimer(4.75f);
}

Enemy2::~Enemy2()
{
}

void Enemy2::SetMineModel(LineModelPoints model)
{
	MineModel = model;
}

void Enemy2::SetSpawnSound(Sound sound)
{
	SpawnSound = sound;

	SetSoundVolume(SpawnSound, 0.5f);
}

void Enemy2::SetOnSound(Sound sound)
{
	OnSound = sound;

	SetSoundVolume(OnSound, 0.25f);
}

void Enemy2::SetMineExplodeSound(Sound sound)
{
	MineExplodeSound = sound;
}

bool Enemy2::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	Enabled = false;
	Points = 1300;

	return false;
}

bool Enemy2::BeginRun()
{
	Enemy::BeginRun();

	SetSoundVolume(ExplodeSound, 0.5f);
	SetSoundVolume(FireSound, 0.5f);

	return false;
}

void Enemy2::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	SetRotateVelocity(Destination, TurnSpeed, Speed);

	if (Managers.EM.TimerElapsed(LayMineTimerID))
	{
		DropMine();
	}

	CheckCollisions();

	if (CheckWentOffScreen()) Enabled = false;
}

void Enemy2::Draw3D()
{
	LineModel::Draw3D();
}

void Enemy2::Spawn()
{
	Managers.EM.ResetTimer(LayMineTimerID, MineDropTimeAmount = 4.75f);

	Enemy::Spawn();
}

void Enemy2::Spawn(Vector3 position)
{
	Entity::Spawn(position);
}

void Enemy2::Hit()
{
	Enemy::Hit();

}

void Enemy2::Destroy()
{
	Enemy::Destroy();

}

void Enemy2::Reset()
{
	Destroy();

	Managers.EM.ResetTimer(LayMineTimerID, MineDropTimeAmount = 4.75f);

	for (const auto& mine : Mines)
	{
		mine->Destroy();
	}
}

bool Enemy2::CheckWentOffScreen()
{
	return Enemy::CheckWentOffScreen();

}

void Enemy2::DestinationTarget()
{
	Enemy::DestinationTarget();

}

void Enemy2::DropMine()
{
	bool spawnNewMine = true;
	size_t mineNumber = Mines.size();

	if (!Player->GameOver) PlaySound(FireSound);

	MineDropTimeAmount = 4 - (((float)Wave - 3) * 0.5f);
	float dropTime = GetRandomFloat(MineDropTimeAmount / 2, MineDropTimeAmount);

	Managers.EM.ResetTimer(LayMineTimerID, dropTime);

	for (size_t i = 0; i < mineNumber; i++)
	{
		if (Mines.at(i)->Enabled == false)
		{
			spawnNewMine = false;
			mineNumber = i;
			break;
		}
	}

	if (spawnNewMine)
	{
		Mines.push_back(DBG_NEW TheMine());
		Managers.EM.AddLineModel(Mines.back());
		Mines.back()->SetPlayer(Player);
		Mines.back()->SetModel(MineModel);
		Mines.back()->SetExplodeSound(ExplodeSound);
		Mines.back()->Initialize(TheUtilities);
		Mines.back()->SetParticleManager(Particles);
		Mines.back()->BeginRun();
	}

	Mines.at(mineNumber)->Spawn(Position);
}

bool Enemy2::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}
