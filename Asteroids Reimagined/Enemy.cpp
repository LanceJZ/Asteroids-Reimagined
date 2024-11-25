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

}

void Enemy::FixedUpdate(float deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	if (!Player->GameOver && !IsSoundPlaying(OnSound)) PlaySound(OnSound);
}

void Enemy::Draw3D()
{
	LineModel::Draw3D();
}

bool Enemy::CheckRockCollisions(Entity* rock)
{
	bool shotHit = false;

	for (const auto& shot : Shots)
	{
		if (!shot->Enabled) continue;

		if (shot->CirclesIntersect(*rock))
		{
			shot->Destroy();
			rock->Hit();
			shotHit = true;
		}
	}

	return shotHit;
}

void Enemy::CheckShotsHitPlayer()
{
	for (const auto shot : Shots)
	{
		if (!shot->Enabled) continue;

		if (shot->CirclesIntersect(*Player))
		{
			Player->Hit(shot->Position, shot->Velocity);
			shot->Destroy();
			break;
		}
	}
}

void Enemy::Spawn()
{
	if (!Player->GameOver) PlaySound(SpawnSound);

	MaxSpeed = 133.666f;

	Vector3 position = { 0.0f, 0.0f, 0.0f };
	int width = (int)(WindowWidth / 1.25f);
	int height = (int)(WindowHeight / 1.25f);

	if (GetRandomValue(1, 10) < 5)
	{
		if (GetRandomValue(1, 10) < 5)
		{
			// Top
			EdgeSpawnedFrom = Edge::Top;
			position.y = (float)-WindowHeight;
			position.x = (float)GetRandomValue(-width, width);
			Velocity.y = MaxSpeed;
			Destination = { position.x, (float)WindowHeight, 0 };
		}
		else
		{
			//Bottom
			EdgeSpawnedFrom = Edge::Bottom;
			position.y = (float)WindowHeight;
			position.x = (float)GetRandomValue(-width, width);
			Velocity.y = -MaxSpeed;
			Destination = { position.x, (float) -WindowHeight, 0};
		}

	}
	else
	{
		if (GetRandomValue(1, 10) < 5)
		{
			//Left
			EdgeSpawnedFrom = Edge::Left;
			position.x = (float)-WindowWidth;
			position.y = (float)GetRandomValue(-height, height);
			Velocity.x = MaxSpeed;
			Destination = { (float)WindowWidth, position.y, 0 };
		}
		else
		{
			//Right
			EdgeSpawnedFrom = Edge::Right;
			position.x = (float)WindowWidth;
			position.y = (float)GetRandomValue(-height, height);
			Velocity.x = -MaxSpeed;
			Destination = { (float) -WindowWidth, position.y, 0};
		}
	}

	Spawn(position);
	RotationZ = GetAngleFromVectorZ(Destination);
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

	Particles.SpawnLineParticles(Position,
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

	Shots[spawnNumber]->Spawn(Position, velocity, 3.0f);
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
	Vector3 closestEnemyPosition = { 0.0f, 0.0f, 0.0f };
	float shortestDistance = 1000.0f;

	for (const auto& enemy : Enemy1Refs)
	{
		if (enemy->Enabled)
		{
			float distance = Vector3Distance(enemy->Position, Position);

			if (distance < shortestDistance)
			{
				shortestDistance = distance;
				closestEnemyPosition = enemy->Position;
			}
		}
	}

	for (const auto& enemy : Enemy2Refs)
	{
		if (enemy->Enabled)
		{
			float distance = Vector3Distance(enemy->Position, Position);

			if (distance < shortestDistance)
			{
				shortestDistance = distance;
				closestEnemyPosition = enemy->Position;
			}
		}
	}

	SetRotateVelocity(closestEnemyPosition, TurnSpeed, Speed);
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

	return ufoActive;
}

bool Enemy::CheckEnemyActive()
{
	bool enemyActive = false;

	if (!Player->Enabled)
	{
		for (const auto& enemy : Enemy1Refs)
		{
			if (enemy->Enabled) enemyActive = true;
		}

		for (const auto& enemy : Enemy2Refs)
		{
			if (enemy->Enabled) enemyActive = true;
		}
	}

	return enemyActive;
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

void Enemy::DestinationTarget()
{
	switch (EdgeSpawnedFrom)
	{
	case Edge::Top:
		DestinationTopBottom();
		break;
	case Edge::Bottom:
		DestinationTopBottom();
		break;
	case Edge::Left:
		DestinationLeftRight();
		break;
	case Edge::Right:
		DestinationLeftRight();
		break;
	}
}

void Enemy::DestinationTopBottom()
{
	if (Player->X() > X())
	{
		Destination.x = (-WindowWidth * 0.5f) + (Player->X() - (WindowWidth * 0.25f));
	}
	else
	{
		Destination.x = (WindowWidth * 0.5f) - (Player->X() - (WindowWidth * 0.25f));
	}
}

void Enemy::DestinationLeftRight()
{
	if (Player->Y() > Y())
	{
		Destination.y = (-WindowHeight * 0.5f) + (Player->Y() - (WindowHeight * 0.25f));
	}
	else
	{
		Destination.y = (WindowHeight * 0.5f) - (Player->Y() - (WindowHeight * 0.25f));
	}
}

bool Enemy::CheckWentOffScreen()
{
	if (EdgeSpawnedFrom == Edge::Right || EdgeSpawnedFrom == Edge::Left)
	{
		if (X() < -WindowWidth)
		{
			return true;
		}
		if (X() > WindowWidth)
		{
			return true;
		}
	}
	else if (EdgeSpawnedFrom == Edge::Top || EdgeSpawnedFrom == Edge::Bottom)
	{
		if (Y() < -WindowHeight)
		{
			return true;
		}
		if (Y() > WindowHeight)
		{
			return true;
		}
	}

	CheckScreenEdge();

	return false;
}

bool Enemy::CheckCollisions()
{
	for (const auto& shot : Player->Shots)
	{
		if (shot->CirclesIntersect(*this))
		{
			shot->Destroy();
			Hit();

			if (Player->GameOver) return true;

			Player->ScoreUpdate(Points);

			return true;
		}
	}

	for (const auto& shot : Player->DoubleShots)
	{
		if (CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();

			if (Player->GameOver) return true;

			Player->ScoreUpdate(Points);

			return true;
		}
	}

	for (const auto& bigShot : Player->BigShots)
	{
		if (CirclesIntersect(*bigShot))
		{
			bigShot->HitPoints -= 50;

			if (bigShot->HitPoints <= 0) bigShot->Destroy();

			Hit();

			if (Player->GameOver) return true;

			Player->ScoreUpdate(Points);

			return true;
		}
	}

	for (const auto& mine : Player->Mines)
	{
		if (!mine->Enabled) continue;

		if (CirclesIntersect(*mine))
		{
			mine->Hit();
			Hit();

			if (Player->GameOver) return true;

			Player->ScoreUpdate(Points);

			return true;
		}
	}

	for (const auto& plasma : Player->PlasmaShots)
	{
		if (!plasma->Enabled) continue;

		if (CirclesIntersect(*plasma))
		{
			Hit();

			if (Player->GameOver) return true;

			Player->ScoreUpdate(Points);

			return true;
		}

		for (const auto& Enemy : Enemy1Refs)
		{
			if (!Enemy->Enabled) continue;

			for (const auto& shot : Enemy->Shots)
			{
				if (!shot->Enabled) continue;

				if (plasma->CirclesIntersect(*shot))
				{
					shot->Destroy();
				}
			}
		}

		for (const auto& Enemy : Enemy2Refs)
		{
			if (!Enemy->Enabled) continue;

			for (const auto& shot : Enemy->Shots)
			{
				if (!shot->Enabled) continue;

				if (plasma->CirclesIntersect(*shot))
				{
					shot->Destroy();
				}
			}
		}

		if (!UFORefs.size()) return false;

		for (const auto& ufo : UFORefs)
		{
			if (!ufo->Enabled) continue;

			for (const auto& shot : ufo->Shots)
			{
				if (!shot->Enabled) continue;

				if (plasma->CirclesIntersect(*shot))
				{
					shot->Destroy();
				}
			}
		}
	}

	if (!Player->GetBeenHit())
	{
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