#include "TheUFO.h"

TheUFO::TheUFO()
{
	FireTimerID = TheManagers.EM.AddTimer(1.25f);
	ChangeVectorTimerID = TheManagers.EM.AddTimer(5.50f);

	for (int i = 0; i < 2; i++)
	{
		Shots[i] = DBG_NEW Shot();
		TheManagers.EM.AddLineModel(Shots[i]);
	}
}

TheUFO::~TheUFO()
{
}

void TheUFO::SetRocks(std::vector<TheRock*> &rocks)
{
	Rocks = &rocks;
}

bool TheUFO::Initialize(Utilities* utilities)
{
	LineModel::Initialize(TheUtilities);

	//Scale = 26.5f;

	return false;
}

bool TheUFO::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheUFO::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void TheUFO::SetShotModel(LineModelPoints model)
{
	for (auto& shot : Shots)
	{
		shot->SetModel(model);
	}
}

void TheUFO::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (CheckReachedSide())
	{
		Enabled = false;
	}

	if (TheManagers.EM.TimerElapsed(FireTimerID))
	{
		FireShot();
	}

	if (TheManagers.EM.TimerElapsed(ChangeVectorTimerID))
	{
		ChangeVector();
	}

	CheckScreenEdgeY();
	CheckCollisions();
}

void TheUFO::Draw3D()
{
	LineModel::Draw3D();
}

void TheUFO::CheckCollisions(TheRock* rock)
{
	for (auto& shot : Shots)
	{
		if (shot->Enabled && shot->CirclesIntersect(*rock))
		{
			shot->Destroy();
			rock->Hit();
			return;
		}

		if (shot->Enabled && shot->CirclesIntersect(*Player))
		{
			shot->Destroy();
			Player->Hit(Position, Velocity);
			return;
		}
	}

	if (Enabled && CirclesIntersect(*rock))
	{
		Hit();
		Destroy();
		rock->Hit();
		return;
	}
}

void TheUFO::Spawn(int spawnCount)
{
	Vector3 position = { 0, 0, 0 };
	int height = WindowHeight / 1.25f;

	position.y = GetRandomValue(-height, height);

	TheManagers.EM.ResetTimer(FireTimerID);
	TheManagers.EM.ResetTimer(ChangeVectorTimerID);

	float fullScale = 1.0f;
	float fullRadius = 18.5f;
	float fullSpeed = 128.666f;
	float spawnPercent = (powf(0.915f, (float)spawnCount / (float)(Wave + 1)) * 100);

	if (GetRandomFloat(0, 99) < spawnPercent - Player->GetScore() / 500)
	{
		TheSize = Large;
		Scale = fullScale;
		MaxSpeed = fullSpeed / 1.333f;
		Radius = fullRadius;
	}
	else
	{
		TheSize = Small;
		Scale = fullScale / 2;
		MaxSpeed = fullSpeed;
		Radius = fullRadius / 2;
	}

	if (GetRandomValue(1, 10) < 5)
	{
		position.x = WindowWidth;
		Velocity.x = -MaxSpeed;
	}
	else
	{
		position.x = -WindowWidth;
		Velocity.x = MaxSpeed;
	}

	Entity::Spawn(position);
}

void TheUFO::Destroy()
{
	Entity::Destroy();

}

void TheUFO::FireShot()
{
	float angle = 0;
	float shotSpeed = 325;
	bool shootRocks = false;
	TheManagers.EM.ResetTimer(FireTimerID);

	if (DeathStarActive)
	{
		angle = AimedShotAtDeathStar();
	}
	else
	{
		if (GetRandomValue(1, 10) < 5 || !Player->Enabled)
		{
			angle = AimedShotAtRock();
			angle = AimedShot();
		}
		else
		{
			angle = AimedShot();
		}
	}

	if (TheUFO::Large)
	{
		if (GetRandomValue(1, 10) < 3)
		{
			angle = GetRandomRadian();
		}
	}

	for (auto shot : Shots)
	{
		if (!shot->Enabled)
		{
			//if (!player->gameOver)
			//{
			//	PlaySound(Sound04);
			//}

			Vector3 offset = Vector3Add(VelocityFromAngleZ(Radius), Position);
			shot->Spawn(offset, GetVelocityFromAngleZ(angle, shotSpeed), 2.5f);

			break;
		}
	}
}

float TheUFO::AimedShot()
{
	if (!Player->Enabled)
	{
		return GetRandomRadian();
	}

	float percentChance = 0.2f - (Player->GetScore() * 0.00001f);

	if (percentChance < 0)
	{
		percentChance = 0;
	}

	float minP = 0.015f - (0.01f * 0.0025f);

	if (minP < 0)
		minP = 0;

	float maxP = 0.02f + minP;

	percentChance += GetRandomFloat(minP, maxP);

	return AngleFromVectorZ(Player->Position) +
		GetRandomFloat(-percentChance, percentChance);
}

float TheUFO::AimedShotAtDeathStar()
{
	return 0.0f;
}

float TheUFO::AimedShotAtRock()
{
	bool noRocks = true;
	Vector3 closestRockPosition = { 0, 0, 0 };
	Vector3 closestRockVelocity = { 0, 0, 0 };
	float shortestDistance = 0.0f;

	for (auto &rock : *Rocks)
	{
		if (rock->Enabled)
		{
			noRocks = false;
			float distance = Vector3Distance(rock->Position, Position);

			if (distance < shortestDistance)
			{
				shortestDistance = distance;
				closestRockPosition = rock->Position;
				closestRockVelocity = rock->Velocity;
			}
		}
	}

	if (noRocks)
	{
		return GetRandomRadian();
	}

	Vector3 dist = GetVelocityFromAngleZ(AngleFromVectorZ(closestRockVelocity),
		shortestDistance);

	return AngleFromVectorZ(Vector3Add(closestRockPosition,
		Vector3Add(closestRockVelocity, dist)));
}

void TheUFO::ChangeVector()
{
	TheManagers.EM.ResetTimer(ChangeVectorTimerID, GetRandomFloat(3.1f, 7.5f));

	if (GetRandomValue(1, 10) > 2)
	{
		if ((int)Velocity.y == 0)
		{
			if (GetRandomValue(1, 10) < 5)
			{
				Velocity.y = MaxSpeed;
			}
			else
			{
				Velocity.y = -MaxSpeed;
			}
		}
		else
		{
			if (Position.y < WindowHeight - (Radius * 3) &&
				Position.y > -WindowHeight - (Radius * 3))
			{
				Velocity.y = 0;
			}
		}
	}
}

bool TheUFO::CheckReachedSide()
{
	if (X() < -WindowWidth)
	{
		return true;
	}
	if (X() > WindowWidth)
	{
		return true;
	}

	return false;
}

bool TheUFO::CheckCollisions()
{
	if (Player->Enabled)
	{
		if (CirclesIntersect(*Player))
		{
			if (!Player->Shield->Enabled) Destroy();

			Player->Hit(Position, Velocity);
			SendScoreToPlayer();

			return true;
		}
	}

	for (auto& shot : Player->Shots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Destroy();
			SendScoreToPlayer();

			return true;
		}
	}

	return false;
}

void TheUFO::SendScoreToPlayer()
{
	switch (TheSize)
	{
	case Large:
		Player->ScoreUpdate(200);
		break;
	case Small:
		Player->ScoreUpdate(1000);
		break;
	}
}
