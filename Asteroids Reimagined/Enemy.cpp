#include "Enemy.h"

Enemy::Enemy()
{
	ShotTimerID = Managers.EM.AddTimer();
}

Enemy::~Enemy()
{
}

void Enemy::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void Enemy::SetUFO(Enemy* ufo[2])
{
	for (int i = 0; i < 2; i++)
	{
		UFOs[i] = ufo[i];
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

	Managers.EM.ResetTimer(ShotTimerID, GetRandomFloat(0.75f, 1.5f));
}

void Enemy::Hit()
{
	Entity::Hit();

	if (!Player->GameOver) PlaySound(ExplodeSound);

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

	Velocity = { 0.0f, 0.0f, 0.0f };
	RotationVelocityZ = 0.0f;

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

	Managers.EM.ResetTimer(ShotTimerID);

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
		Managers.EM.AddLineModel(Shots[spawnNumber], ShotModel);
		Shots[spawnNumber]->BeginRun();
	}

	Shots[spawnNumber]->Spawn(Position);
}

void Enemy::Shoot(Vector3 velocity)
{
	Managers.EM.ResetTimer(ShotTimerID);

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
		Managers.EM.AddLineModel(Shots[spawnNumber], ShotModel);
		Shots[spawnNumber]->BeginRun();
	}

	Shots[spawnNumber]->Spawn(Position, velocity);
}

bool Enemy::CheckCollisions()
{
	for (const auto& shot : Player->Shots)
	{
		if (shot->Enabled && shot->CirclesIntersect(*this))
		{
			shot->Destroy();
			Hit();
			Player->ScoreUpdate(Points);

			return true;
		}
	}

	for (const auto& shot : Player->DoubleShots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			Player->ScoreUpdate(Points);

			return true;
		}
	}

	for (const auto& bigShot : Player->BigShots)
	{
		if (bigShot->Enabled && CirclesIntersect(*bigShot))
		{

			bigShot->HitPoints -= 50;

			if (bigShot->HitPoints <= 0) bigShot->Destroy();

			Hit();
			Player->ScoreUpdate(Points);

			return true;
		}
	}

	for (const auto& mine : Player->Mines)
	{
		if (mine->Enabled && CirclesIntersect(*mine))
		{
			mine->Hit();
			Hit();
			Player->ScoreUpdate(Points);

			return true;
		}
	}

	for (const auto& plasma : Player->PlasmaShots)
	{
		if (plasma->Enabled && CirclesIntersect(*plasma))
		{
			Hit();
			Player->ScoreUpdate(Points);

			return true;
		}
	}

	if (CirclesIntersect(*Player))
	{
		if (!Player->Shield->Enabled)
		{
			Hit();
			Player->ScoreUpdate(Points);
		}

		Player->Hit(Position, Velocity);

		return true;
	}

	if (UFOs[0] == nullptr) return false;

	for (const auto& ufo : UFOs)
	{
		for (const auto& shot : ufo->Shots)
		{
			if (!shot->Enabled) continue;

			if (CirclesIntersect(*shot))
			{
				shot->Destroy();
				Hit();
				return true;
			}
		}

		if (!ufo->Enabled) continue;

		if (ufo->CirclesIntersect(*this))
		{
			ufo->Hit();
			Hit();

			return true;
		}
	}

	return false;
}

void Enemy::ChasePlayer()
{
	RotationVelocityZ = 0.0f;

	if (!Player->Enabled) return;

	SetRotateVelocity(Player->Position, TurnSpeed, Speed);
}

void Enemy::ChaseUFO()
{
	if (UFOs[0]->Enabled && UFOs[1]->Enabled)
	{
		UFOs[0]->Distance =	Vector3Distance(UFOs[0]->Position, Position);
		UFOs[1]->Distance =	Vector3Distance(UFOs[1]->Position, Position);

		if (UFOs[0]->Distance <	UFOs[1]->Distance && UFOs[0]->Enabled)
		{
			SetRotateVelocity(UFOs[0]->Position, TurnSpeed, Speed);
		}
		else if (UFOs[1]->Distance < UFOs[0]->Distance && UFOs[1]->Enabled)
		{
			SetRotateVelocity(UFOs[1]->Position, TurnSpeed, Speed);
		}
	}
	else if (UFOs[0]->Enabled)
	{
		SetRotateVelocity(UFOs[0]->Position, TurnSpeed, Speed);
	}
	else if (UFOs[1]->Enabled)
	{
		SetRotateVelocity(UFOs[1]->Position, TurnSpeed, Speed);
	}
}