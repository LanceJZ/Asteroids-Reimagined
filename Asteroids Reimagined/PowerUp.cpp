#include "PowerUp.h"

PowerUp::PowerUp()
{
	LifeTimerID = TheManagers.EM.AddTimer(15.0f);
}

PowerUp::~PowerUp()
{
}

void PowerUp::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void PowerUp::SetPickUpSound(Sound sound)
{
	PickUpSound = sound;
}

bool PowerUp::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool PowerUp::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void PowerUp::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (TheManagers.EM.TimerElapsed(LifeTimerID))
	{
		Destroy();
	}

	CheckCollisions();
}

void PowerUp::Draw3D()
{
	LineModel::Draw3D();
}

void PowerUp::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	TheManagers.EM.ResetTimer(LifeTimerID);

	if (GetRandomValue(1, 10) < 5)
	{
		Shield = true;
		ModelColor = BLUE;
	}
	else
	{
		Heat = true;
		ModelColor = RED;
	}
}

void PowerUp::Destroy()
{
	Entity::Destroy();

}

void PowerUp::CheckCollisions()
{
	if (Player->Enabled && CirclesIntersect(*Player))
	{
		PlaySound(PickUpSound);

		if (Shield) Player->ShieldPowerUp();

		if (Heat) Player->HeatPowerUp();

		Destroy();
	}
}
