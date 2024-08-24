#include "PowerUp.h"

PowerUp::PowerUp()
{
	LifeTimerID = Managers.EM.AddTimer(15.0f);
}

PowerUp::~PowerUp()
{
}

void PowerUp::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void PowerUp::SetEnemy(EnemyControl* enemies)
{
	Enemies = enemies;
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

	if (Managers.EM.TimerElapsed(LifeTimerID))
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

	Managers.EM.ResetTimer(LifeTimerID);

	int chance = GetRandomValue(1, 10);

	if (chance < 5)
	{
		ModelColor = BLUE;
		Type = Blue; //Shield over charge.
	}
	else if (chance < 9)
	{
		ModelColor = RED;
		Type = Red; //Gun cooling over charge.
	}
	else
	{
		ModelColor = PURPLE;
		Type = Purple; //Everything over charged for a time.
	}

	return; //For testing purposes.

	if (Enemies->Wave > 2)
	{
		if (chance == 7)
		{
			ModelColor = GREEN;
			Type = Green; //Larger Shots limited number.
		}

		if (chance == 8)
		{
			ModelColor = YELLOW;
			Type = Yellow; //Double Shots limited number.
		}
	}

	if (Enemies->Wave > 4)
	{
		if (chance == 1)
		{
			ModelColor = ORANGE;
			Type = Orange; //Ion Mines limited number.
		}

		if (chance == 2)
		{
			ModelColor = VIOLET;
			Type = Violet; //Homing missiles limited number.
		}
	}

	if (Enemies->Wave > 6)
	{
		if (chance < 3)
		{
			ModelColor = MAGENTA;
			Type = Magenta;
		}
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

		switch (Type)
		{
		case PowerUp::Red:
			Player->GunPowerUp();
			break;
		case PowerUp::Blue:
			Player->ShieldPowerUp();
			break;
		case PowerUp::Purple:
			Player->FullPowerUp();
			break;
		default:
			break;
		}

		Destroy();
	}
}
