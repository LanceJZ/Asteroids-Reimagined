#include "Enemy2.h"

Enemy2::Enemy2()
{
	LayMineTimerID = TheManagers.EM.AddTimer(4.75f);
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

	if (TheManagers.EM.TimerElapsed(LayMineTimerID))
	{
		TheManagers.EM.ResetTimer(LayMineTimerID, GetRandomFloat(2.75f, 5.5f));
		DropMine();
	}

	CheckCollisions();

	if (CheckWentOffScreen()) Enabled = false;

	if (!Player->GameOver && !IsSoundPlaying(OnSound)) PlaySound(OnSound);
}

void Enemy2::Draw3D()
{
	LineModel::Draw3D();
}

void Enemy2::Spawn(Vector3 position)
{
	TheManagers.EM.ResetTimer(LayMineTimerID);

	if (!Player->GameOver) PlaySound(SpawnSound);

	int width = (int)(WindowWidth / 1.25f);
	int height = (int)(WindowHeight / 1.25f);

	MaxSpeed = 75.666f;

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

	for (const auto& mine : Mines)
	{
		mine->Destroy();
	}
}

bool Enemy2::CheckWentOffScreen()
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

void Enemy2::DestinationTarget()
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

void Enemy2::DestinationTop()
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

void Enemy2::DestinationBottom()
{
	DestinationTop();
}

void Enemy2::DestinationLeft()
{
	if (Player->Y() > Y())
	{
		Destination.y = ((float) -WindowHeight * 0.5f) + (Player->Y()
			- ((float)WindowHeight * 0.25f));
	}
	else
	{
		Destination.y = ((float)WindowHeight * 0.5f) - (Player->Y()
			- ((float)WindowHeight * 0.25f));
	}
}

void Enemy2::DestinationRight()
{
	DestinationLeft();
}

void Enemy2::DropMine()
{
	bool spawnNewMine = true;
	size_t mineNumber = Mines.size();

	if (!Player->GameOver) PlaySound(FireSound);

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
		TheManagers.EM.AddLineModel(Mines.back());
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
