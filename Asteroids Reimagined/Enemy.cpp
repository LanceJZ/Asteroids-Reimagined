#include "Enemy.h"

Enemy::Enemy()
{
	ShotTimerID = TheManagers.EM.AddTimer();
}

Enemy::~Enemy()
{
}

void Enemy::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void Enemy::SetShotModel(LineModelPoints model)
{
	ShotModel = model;
}

void Enemy::SetFireSound(Sound fireSound)
{
	FireSound = fireSound;
}

void Enemy::SetExplodeSound(Sound explodeSound)
{
	ExplodeSound = explodeSound;
}

void Enemy::SetParticleManager(ParticleManager* particleManager)
{
	PM = particleManager;
}

bool Enemy::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool Enemy::BeginRun()
{
	LineModel::BeginRun();

	TheManagers.EM.ResetTimer(ShotTimerID, GetRandomFloat(0.75f, 1.5f));

	return false;
}

void Enemy::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	CheckCollision();
}

void Enemy::Draw()
{
	LineModel::Draw();
}

void Enemy::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void Enemy::Hit()
{
	Entity::Hit();

	PlaySound(ExplodeSound);

	Player->ScoreUpdate(Points);
	Destroy();

	PM->Spawn(Position, Vector3Multiply(Velocity, {0.5f}),
		20, 100, 20, 2.0f, { 255, 0, 0, 255 });
}

void Enemy::Reset()
{
	for (auto& shot : Shots)
	{
		shot->Destroy();
	}

	Destroy();
}

void Enemy::Destroy()
{
	Entity::Destroy();

	Enabled = false;
}

void Enemy::Shoot()
{
	PlaySound(FireSound);

	TheManagers.EM.ResetTimer(ShotTimerID);

	bool spawnNew = true;
	size_t spawnNumber = Shots.size();

	for (size_t check = 0; check < spawnNumber; check++)
	{
		if (!Shots[check]->Enabled)
		{
			spawnNew = false;
			spawnNumber = check;
			break;
		}
	}

	if (spawnNew)
	{
		Shots.push_back(DBG_NEW Shot());
		TheManagers.EM.AddLineModel(Shots[spawnNumber], ShotModel);
		Shots[spawnNumber]->Initialize(TheUtilities);
		Shots[spawnNumber]->BeginRun();
	}

	Shots[spawnNumber]->Spawn(Position);
}

void Enemy::Shoot(Vector3 velocity)
{
	TheManagers.EM.ResetTimer(ShotTimerID);

	bool spawnNew = true;
	size_t spawnNumber = Shots.size();

	for (size_t check = 0; check < spawnNumber; check++)
	{
		if (!Shots[check]->Enabled)
		{
			spawnNew = false;
			spawnNumber = check;
			break;
		}
	}

	if (spawnNew)
	{
		Shots.push_back(DBG_NEW Shot());
		TheManagers.EM.AddLineModel(Shots[spawnNumber], ShotModel);
		Shots[spawnNumber]->Initialize(TheUtilities);
		Shots[spawnNumber]->BeginRun();
	}

	Shots[spawnNumber]->Spawn(Position, velocity);
}

bool Enemy::CheckCollision()
{
	if (CirclesIntersect(*Player))
	{
		Hit();

		Player->Hit();

		return true;
	}

	for (auto& shot : Shots)
	{
		if (!shot->Enabled) continue;

		if (shot->CirclesIntersect(*Player))
		{
			shot->Destroy();
			Player->Hit();

			return true;
		}
	}

	//for (auto &shot : Player->Shots)
	//{
	//	if (!shot->Enabled) continue;

	//	if (shot->CirclesIntersectBullet(*this))
	//	{
	//		Hit();
	//		shot->Destroy();

	//		return true;
	//	}
	//}

	return false;
}
