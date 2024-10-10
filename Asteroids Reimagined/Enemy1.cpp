#include "Enemy1.h"

Enemy1::Enemy1()
{
	FireMissileTimerID = Managers.EM.AddTimer(4.0f);
}

Enemy1::~Enemy1()
{
}

void Enemy1::SetPlayer(ThePlayer* player)
{
	Enemy::SetPlayer(player);

}

void Enemy1::SetMissileModel(LineModelPoints model)
{
	MissileModel = model;
}

void Enemy1::SetSpawnSound(Sound sound)
{
	SpawnSound = sound;

	SetSoundVolume(SpawnSound, 0.5f);
}

void Enemy1::SetOnSound(Sound sound)
{
	OnSound = sound;

	SetSoundVolume(OnSound, 0.25f);
}

void Enemy1::SetMissileOnSound(Sound sound)
{
	MissileOnSound = sound;
}

void Enemy1::SetMissileExplodeSound(Sound sound)
{
	MissileExplodeSound = sound;
}

bool Enemy1::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	Enabled = false;
	Points = 1250;

	return false;
}

bool Enemy1::BeginRun()
{
	Enemy::BeginRun();

	SetSoundVolume(ExplodeSound, 0.5f);
	SetSoundVolume(FireSound, 0.5f);

	return false;
}

void Enemy1::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	SetRotateVelocity(Destination, TurnSpeed, Speed);

	if (Managers.EM.TimerElapsed(FireMissileTimerID))
	{
		FireMissile();
	}

	DestinationTarget();
	CheckCollisions();

	if (CheckWentOffScreen()) Enabled = false;
}

void Enemy1::Draw3D()
{
	LineModel::Draw3D();
}

void Enemy1::Spawn()
{
	Managers.EM.ResetTimer(FireMissileTimerID, MissileFireTimerAmount = 6.5f);

	MaxSpeed = 133.666f;
	MissilesFired = 0;

	Enemy::Spawn();
}

void Enemy1::Spawn(Vector3 position)
{
	Enemy::Spawn(position);
}

void Enemy1::Hit()
{
	Enemy::Hit();

}

void Enemy1::Destroy()
{
	Entity::Destroy();

}

void Enemy1::Reset()
{
	Destroy();

	Managers.EM.SetTimer(FireMissileTimerID, MissileFireTimerAmount = 6.5f);

	for (const auto& missile : Missiles)
	{
		missile->Destroy();
	}
}

bool Enemy1::CheckWentOffScreen()
{
	return Enemy::CheckWentOffScreen();

}

void Enemy1::DestinationTarget()
{
	Enemy::DestinationTarget();

}

void Enemy1::FireMissile() //TODO: Move common parts to Enemy class.
{
	float missileTimeAmountAdjust = ((float)(Wave - 3) * 0.5f) +
		((((float)MissilesFired++)) * 0.05f);

	if (MissileFireTimerAmount < missileTimeAmountAdjust - 0.2f)
		missileTimeAmountAdjust = MissileFireTimerAmount - 0.2f;

	float min = (MissileFireTimerAmount - missileTimeAmountAdjust) /
		((((float)Wave - 3) * 0.5f) + 1.0f);
	float max = MissileFireTimerAmount - missileTimeAmountAdjust;

	if (min < 0.15f) min = 0.15f;

	if (min > max) max = min * 2.0f;

	float missileTime = GetRandomFloat(min,	max);

	Managers.EM.ResetTimer(FireMissileTimerID, missileTime);

	if (!Player->GameOver) PlaySound(FireSound);

	bool spawnMissile = true;
	size_t missileNumber = Missiles.size();

	for (size_t i = 0; i < missileNumber; i++)
	{
		if (!Missiles.at(i)->Enabled)
		{
			spawnMissile = false;
			missileNumber = i;
			break;
		}
	}

	if (spawnMissile)
	{
		Missiles.push_back(DBG_NEW TheMissile());
		Managers.EM.AddLineModel(Missiles.back(), MissileModel);
		Missiles.back()->SetPlayer(Player);
		Missiles.back()->SetParticleManager(Particles);
		Missiles.back()->SetOnSound(MissileOnSound);
		Missiles.back()->SetExplodeSound(MissileExplodeSound);
		Missiles.back()->BeginRun();
	}

	Missiles.at(missileNumber)->Spawn(Position);
	Missiles.at(missileNumber)->RotationZ = RotationZ;

	for (const auto& missile : Missiles)
	{
		missile->UFORefs.clear();
	}

	for (const auto& missiles : Missiles)
	{
		for (const auto& ufo : UFORefs)
		{
			missiles->UFORefs.push_back(ufo);
		}
	}
}

bool Enemy1::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}