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

	DriftToPlayer();
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

	int chance = GetRandomValue(1, 10); //Rework this. Later waves limit Purple.

	if (chance < 5)
	{
		ModelColor = BLUE;
		Type = Blue; //Shield over charge.
	}
	else if (chance < 9)
	{
		ModelColor = SKYBLUE;
		Type = Skyblue; //Gun cooling over charge.
	}
	else
	{
		if (Wave > 1)
		{
			if (GetRandomValue(1, 100) < (100 - (int)(Wave * 4.75f))) return;
		}

		ModelColor = PURPLE;
		Type = Purple; //Everything over charged for a time.
	}

	if (Enemies->Wave > 1)
	{
		if (chance == 7)
		{
			ModelColor = RED;
			Type = Red; //Larger Shots limited number.
		}

		if (chance == 8)
		{
			ModelColor = YELLOW;
			Type = Yellow; //Two side by side shots limited number.
		}
	}

	if (Enemies->Wave > 2)
	{
		if (chance == 1)
		{
			ModelColor = ORANGE;
			Type = Orange; //Homing Mines limited number.
			// Only works on enemy ships, rocks ignored. (Explodes on impact)
		}

		if (chance == 2)
		{
			ModelColor = VIOLET;
			Type = Violet; //Growing plasma shot limited number.
			// Dies at edge of screen. Annihilates everything in it's path.
		}
	}

	return;

	if (Enemies->Wave > 3)
	{
		if (chance == 3)
		{
			ModelColor = MAGENTA;
			Type = Magenta; //Spread Shot limited number.
		}

		if (chance == 4)
		{
			ModelColor = MAROON;
			Type = Maroon; //Homing missiles limited number.
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
		case PowerUp::Skyblue:
			Player->GunPowerUp();
			break;
		case PowerUp::Blue:
			Player->ShieldPowerUp();
			break;
		case PowerUp::Purple:
			Player->FullPowerUp();
			break;
		case PowerUp::Red:
			Player->BigShotPowerUp();
			break;
		case PowerUp::Yellow:
			Player->DoubleShotPowerUp();
			break;
		case PowerUp::Orange:
			Player->MinePowerUp();
			break;
		case PowerUp::Violet:
			Player->PlasmaShotPowerUp();
			break;
		default:
			break;
		}

		Destroy();
	}
}

void PowerUp::DriftToPlayer()
{
	Vector3 pos = Player->Position;

	if (!Player->Enabled) pos = { 0.0f, 0.0f, 0.0f };

	RotationZ = GetAngleFromVectors(pos);
	Velocity = GetVelocityFromAngleZ(36.666f);
}
