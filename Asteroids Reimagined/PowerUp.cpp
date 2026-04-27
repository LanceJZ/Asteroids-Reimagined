#include "PowerUp.h"

PowerUp::PowerUp()
{
	LifeTimerID = EM.AddTimer(30.0f);
	DriftTimerID = EM.AddTimer(1.666f);
}

PowerUp::~PowerUp()
{
}

void PowerUp::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void PowerUp::SetAntiPlayer(TheAntiPlayer* player)
{
	AntiPlayer = player;
}

void PowerUp::SetPickUpSound(Sound sound)
{
	PickUpSound = sound;
}

bool PowerUp::Initialize()
{
	LineModel::Initialize();

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

	CheckCollisions();
}

void PowerUp::FixedUpdate(float deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	if (EM.TimerElapsed(LifeTimerID))
	{
		Destroy();
	}

	DriftToPlayer();
}

void PowerUp::Draw3D()
{
	LineModel::Draw3D();
}

void PowerUp::Spawn(Vector3 position, Vector3 velocity, PowerUpType type, int wave)
{
	Entity::Spawn(position);

	if (wave > 1) EM.ResetTimer(LifeTimerID, 25);
	else if (wave > 3) EM.ResetTimer(LifeTimerID, 20);
	else if (wave > 5) EM.ResetTimer(LifeTimerID, 15);
	else EM.ResetTimer(LifeTimerID, 30);

	Type = type;

	Velocity = velocity;

	if (type == Skyblue) ModelColor = SKYBLUE;
	else if (type == Blue) ModelColor = BLUE;
	else if (type == Purple) ModelColor = PURPLE;
	else if (type == Red) ModelColor = RED;
	else if (type == Yellow) ModelColor = YELLOW;
	else if (type == Orange) ModelColor = ORANGE;
	else if (type == Violet) ModelColor = VIOLET;
	else if (type == Magenta) ModelColor = MAGENTA;
	else if (type == Maroon) ModelColor = MAROON;
}

void PowerUp::Destroy()
{
	Entity::Destroy();

}

void PowerUp::CheckCollisions()
{
	if (!Enabled) return;

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

	if (EM.TimerElapsed(DriftTimerID))
	{
		EM.ResetTimer(DriftTimerID, M.GetRandomFloat(0.666f, 1.666f));
		SetAccelerationToMaxAtRotation(0.666f, 24.666f);
	}
}
