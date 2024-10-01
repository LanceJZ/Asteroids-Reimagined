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

	if (!Player->GameOver && !IsSoundPlaying(OnSound)) PlaySound(OnSound);
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

	if (Particles != nullptr) Particles->SpawnLineParticles(Position,
		Vector3Multiply(Velocity, {0.25f}),
		20, 100, 20, 2.0f, WHITE);

	Destroy();
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

void Enemy::ChasePlayer()
{
	RotationVelocityZ = 0.0f;

	Vector3 target = Player->Position;

	if (!Player->Enabled)
	{
		if (X() > 0.0f) target.x = (float)WindowWidth;
		else target.x = (float)-WindowWidth;

		if (Y() > 0.0f) target.y = (float)WindowHeight;
		else target.y = (float)-WindowHeight;
	}

	SetRotateVelocity(target, TurnSpeed, Speed);
}

void Enemy::ChaseUFO()
{
	Vector3 closestUFOPosition = { 0.0f, 0.0f, 0.0f };
	float shortestDistance = 1000.0f;

	for (const auto& ufo : UFORefs)
	{
		if (ufo->Enabled)
		{
			float distance = Vector3Distance(ufo->Position, Position);

			if (distance < shortestDistance)
			{
				shortestDistance = distance;
				closestUFOPosition = ufo->Position;
			}
		}
	}

	SetRotateVelocity(closestUFOPosition, TurnSpeed, Speed);
}

void Enemy::ChaseEnemy()
{
	//EnemyOne->Distance = Vector3Distance(EnemyOne->Position, Position);
	//EnemyTwo->Distance = Vector3Distance(EnemyTwo->Position, Position);

	//if (EnemyOne->Distance < EnemyTwo->Distance) ChaseEnemyOne();
	//else ChaseEnemyTwo();
}

bool Enemy::CheckUFOActive()
{
	bool ufoActive = false;

	if (!Player->Enabled)
	{
		for (const auto& ufo : UFORefs)
		{
			if (ufo->Enabled) ufoActive = true;
		}
	}
	else ChasePlayer();

	return ufoActive;
}

bool Enemy::LeaveScreen()
{
	LeavePlay(TurnSpeed, Speed);

	if (IsOffScreen())
	{
		Reset();
		true;
	}

	return false;
}

void Enemy::ChaseEnemyOne()
{
	//SetRotateVelocity(EnemyOne->Position, TurnSpeed, Speed);
}

void Enemy::ChaseEnemyTwo()
{
	//SetRotateVelocity(EnemyTwo->Position, TurnSpeed, Speed);
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

	if (!UFORefs.size()) return false;

	for (const auto& ufo : UFORefs)
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