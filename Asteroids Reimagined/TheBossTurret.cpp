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

void TheBossTurret::SetShotModel(LineModelPoints model)
{
	ShotModel = model;
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

void TheBossTurret::Hit()
{
	LineModel::Hit();

	Destroy();
}

void TheBossTurret::Destroy()
{
	Entity::Destroy();
	RotationZ = 0.0f;
}

void TheBossTurret::CheckCollisions()
{
	for (const auto& shot : Shots)
	{
		if (shot->Enabled && shot->CirclesIntersect(*Player) && Player->Enabled)
		{
			shot->Destroy();
			Player->Hit(shot->Position, shot->Velocity);
			break;
		}
	}
}

void TheBossTurret::Fire()
{
	FireTimerSetting = GetRandomFloat(0.75f, 1.5f);
	Managers.EM.ResetTimer(FireTimerID, FireTimerSetting);

	if (!Player->Enabled) return;

	//if (!Player->GameOver) PlaySound(FireSound);

	bool spawnNewShot = true;
	size_t shotNumber = Shots.size();

	for (size_t check = 0; check < shotNumber; check++)
	{
		if (!Shots.at(check)->Enabled)
		{
			spawnNewShot = false;
			shotNumber = check;
			break;
		}
	}

	if (spawnNewShot)
	{
		Shots.push_back(DBG_NEW Shot());
		Managers.EM.AddLineModel(Shots.back(), ShotModel);
		Shots.back()->SetModel(ShotModel);
		Shots.back()->Initialize(TheUtilities);
		Shots.back()->BeginRun();
	}

	float percentChance = 0.5f - (Player->GetScore() * 0.00001f);

	if (percentChance < 0) percentChance = 0;

	float angle = AngleFromWorldVectorZ(Player->Position) +
		GetRandomFloat(-percentChance, percentChance);
	RotationZ = angle -Parents->at(0)->RotationZ;
	float shotSpeed = 200;

	Vector3 offset = Vector3Add(VelocityFromAngleZ(Radius), GetWorldPosition());
	Shots.at(shotNumber)->Spawn(offset, GetVelocityFromAngleZ(angle, shotSpeed), 4.75f);
}
