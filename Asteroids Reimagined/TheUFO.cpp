#include "TheUFO.h"

TheUFO::TheUFO()
{
	FireTimerID = TheManagers.EM.AddTimer();
	ChangeVectorTimerID = TheManagers.EM.AddTimer();

	for (int i = 0; i < 2; i++)
	{
		Shots[i] = DBG_NEW Shot();
		TheManagers.EM.AddLineModel(Shots[i]);
	}
}

TheUFO::~TheUFO()
{
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

	TheManagers.EM.SetTimer(FireTimerID, 5.0f);
	TheManagers.EM.SetTimer(ChangeVectorTimerID, 7.0f);

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
}

void TheUFO::Draw3D()
{
	LineModel::Draw3D();
}

void TheUFO::Spawn(int spawnCount)
{
	Vector3 position = { 600, 0, 0 };

	Entity::Spawn(position);

	TheManagers.EM.ResetTimer(FireTimerID);
	TheManagers.EM.ResetTimer(ChangeVectorTimerID);

	Velocity.x = -100;
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

	if (DeathStarActive)
	{
		angle = AimedShotAtDeathStar();
	}
	else
	{
		if (GetRandomValue(1, 10) < 5 || !Player->Enabled)
		{
			angle = AimedShotAtRock();
		}
		else
		{
			angle = AimedShot();
		}
	}

	if (TheUFO::Large)
	{
		if (GetRandomValue(1, 10) < 2)
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
	return 0.0f;
}

float TheUFO::AimedShotAtDeathStar()
{
	return 0.0f;
}

float TheUFO::AimedShotAtRock()
{
	return 0.0f;
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
