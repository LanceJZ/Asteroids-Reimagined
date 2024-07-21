#include "TheBossTurret.h"

TheBossTurret::TheBossTurret()
{
	FireTimerID = Managers.EM.AddTimer();
}

TheBossTurret::~TheBossTurret()
{
}

void TheBossTurret::SetPlayer(ThePlayer* player)
{
	Player = player;
}

bool TheBossTurret::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool TheBossTurret::BeginRun()
{
	LineModel::BeginRun();

	FireTimerSetting = GetRandomFloat(0.75f, 1.5f);

	return false;
}

void TheBossTurret::Update(float deltaTime)
{
	LineModel::Update(deltaTime);


	if (Managers.EM.TimerElapsed(FireTimerID))
	{
		Fire();
	}
}

void TheBossTurret::Draw3D()
{
	LineModel::Draw3D();
}

void TheBossTurret::Spawn()
{
	Enabled = true;

	Managers.EM.ResetTimer(FireTimerID, FireTimerSetting);
}

void TheBossTurret::Destroy()
{
	Entity::Destroy();
	RotationZ = 0.0f;
}

void TheBossTurret::Fire()
{
	Managers.EM.ResetTimer(FireTimerID, FireTimerSetting);

	if (!Player->Enabled) return;

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

	float angle = AngleFromVectorZ(Player->Position) +
		GetRandomFloat(-percentChance, percentChance);

	RotationZ = angle - Parents->at(0)->RotationZ;

	float shotSpeed = 200;

	for (const auto& shot : Shots)
	{
		if (!shot->Enabled)
		{
			//if (!Player->GameOver) PlaySound(FireSound);

			Vector3 offset = Vector3Add(VelocityFromAngleZ(Radius), Position);
			shot->Spawn(offset, GetVelocityFromAngleZ(angle, shotSpeed), 2.5f);

			break;
		}
	}
}
