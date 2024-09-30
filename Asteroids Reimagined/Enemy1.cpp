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

void Enemy1::Spawn(Vector3 position)
{
	Managers.EM.ResetTimer(FireMissileTimerID);

	if (!Player->GameOver) PlaySound(SpawnSound);

	MaxSpeed = 133.666f;
	MissilesFired = 0;

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

	Enemy::Spawn(position);
	RotationZ = GetAngleFromVectorZ(Destination);
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

void Enemy1::DestinationTarget()
{
	switch (EdgeSpawnedFrom)
	{
	case Edge::Top:
		DestinationBottom();
		break;
	case Edge::Bottom:
		DestinationTop();
		break;
	case Edge::Left:
		DestinationRight();
		break;
	case Edge::Right:
		DestinationLeft();
		break;
	}
}

void Enemy1::DestinationTop()
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

void Enemy1::DestinationBottom()
{
	DestinationTop();
}

void Enemy1::DestinationLeft()
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

void Enemy1::DestinationRight()
{
	DestinationLeft();
}

void Enemy1::FireMissile()
{
	float missileTimeAmountAdjust = ((float)(Wave - 3) * 0.1f) +
		((((float)MissilesFired++)) * 0.1f);

	if (MissileFireTimerAmount < missileTimeAmountAdjust - 0.2f)
		missileTimeAmountAdjust = MissileFireTimerAmount - 0.2f;

	float min = MissileFireTimerAmount / ((((float)Wave - 3) * 0.1f) + 1.0f);
	float max = MissileFireTimerAmount - missileTimeAmountAdjust;

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
		Managers.EM.AddEntity(Missiles.back());
		Missiles.back()->SetModel(MissileModel);
		Missiles.back()->SetPlayer(Player);
		Missiles.back()->SetParticleManager(Particles);
		Missiles.back()->SetOnSound(MissileOnSound);
		Missiles.back()->SetExplodeSound(MissileExplodeSound);
		Missiles.back()->Initialize(TheUtilities);
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
