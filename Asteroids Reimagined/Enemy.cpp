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

void Enemy::SetUFO(TheUFO* ufos[2])
{
	for (int i = 0; i < 2; i++)
	{
		UFOs[i] = ufos[i];
	}
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
	Particles = particleManager;
}

bool Enemy::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool Enemy::BeginRun()
{
	LineModel::BeginRun();


	return false;
}

void Enemy::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	//CheckCollision();
}

void Enemy::Draw3D()
{
	LineModel::Draw3D();
}

void Enemy::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	TheManagers.EM.ResetTimer(ShotTimerID, GetRandomFloat(0.75f, 1.5f));
}

void Enemy::Hit()
{
	Entity::Hit();

	if (!Player->GameOver) PlaySound(ExplodeSound);



	//Player->ScoreUpdate(Points);
	Destroy();

	Particles->SpawnLineParticles(Position, Vector3Multiply(Velocity, {0.25f}),
		20, 100, 20, 2.0f, WHITE);
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
		Shots[spawnNumber]->BeginRun();
	}

	Shots[spawnNumber]->Spawn(Position, velocity);
}

bool Enemy::CheckCollisions()
{
	for (auto& playerShot : Player->Shots)
	{
		if (playerShot->Enabled && playerShot->CirclesIntersect(*this))
		{
			playerShot->Destroy();
			Hit();
			Destroy();
			Player->ScoreUpdate(Points);

			return true;
		}
	}

	if (Player->Enabled && Player->CirclesIntersect(*this))
	{
		Player->Hit(Position, Velocity);

		if (!Player->Shield->Enabled)
		{
			Hit();
			Destroy();
			Player->ScoreUpdate(Points);
		}

		return true;
	}

	for (auto& ufo : UFOs)
	{
		if (ufo->Enabled && ufo->CirclesIntersect(*this))
		{
			ufo->Hit();
			Hit();
			Destroy();
			return true;
		}

		for (auto& shot : ufo->Shots)
		{
			if (shot->Enabled && shot->CirclesIntersect(*this))
			{
				shot->Destroy();
				Hit();
				Destroy();
				return true;
			}
		}
	}

	return false;
}

void Enemy::ChasePlayer()
{
	SetRotateVelocity(Player->Position, TurnSpeed, Speed);
}
