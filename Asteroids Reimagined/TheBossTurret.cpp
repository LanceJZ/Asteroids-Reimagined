#include "TheBossTurret.h"

TheBossTurret::TheBossTurret()
{
	FireTimerID = EM.AddTimer();
}

TheBossTurret::~TheBossTurret()
{
}

void TheBossTurret::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void TheBossTurret::SetShotModel(std::vector<Vector3> model)
{
	ShotModel = model;
}

void TheBossTurret::SetFireSound(Sound sound)
{
	FireSound = sound;
	SetSoundVolume(FireSound, 0.25f);
}

void TheBossTurret::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
}

bool TheBossTurret::Initialize()
{
	LineModel::Initialize();

	return false;
}

bool TheBossTurret::BeginRun()
{
	LineModel::BeginRun();

	FireTimerSetting = M.GetRandomFloat(0.75f, 1.5f);

	return false;
}

void TheBossTurret::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void TheBossTurret::FixedUpdate(float deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	if (EM.TimerElapsed(FireTimerID))
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

	EM.ResetTimer(FireTimerID, FireTimerSetting * 3.5f);
}

void TheBossTurret::Hit()
{
	LineModel::Hit();

	if (!Player->GameOver) PlaySound(ExplodeSound);

	Particles.SpawnLineDots(GetWorldPosition(),
		Vector3Multiply(Velocity, {0.25f}),
		20, 100, 20, 2.0f, WHITE);

	Destroy();
}

void TheBossTurret::Destroy()
{
	Entity::Destroy();
	RotationZ = 0.0f;
}

void TheBossTurret::Reset()
{
	Destroy();

	for (const auto& shot : Shots)
	{
		shot->Destroy();
	}
}

void TheBossTurret::CheckCollisions()
{
	for (const auto& shot : Shots)
	{
		if (!shot->Enabled) continue;

		if (Player->Shield->Enabled &&
			CirclesIntersect(Player->Position, Player->Shield->Radius))
		{
			Player->ShieldHit(shot->Position, shot->Velocity);
			shot->Destroy();
			break;
		}

		if (shot->CirclesIntersect(*Player) && !Player->Shield->Enabled)
		{
			shot->Destroy();
			Player->Hit();
			break;
		}
	}
}

void TheBossTurret::Fire()
{
	FireTimerSetting = M.GetRandomFloat(0.75f, 1.5f);
	EM.ResetTimer(FireTimerID, FireTimerSetting);

	if (!Player->Enabled) return;

	if (!Player->GameOver) PlaySound(FireSound);

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
		EM.AddLineModel(Shots.back(), ShotModel);
		Shots.back()->SetModel(ShotModel);
		Shots.back()->BeginRun();
	}

	float percentChance = 0.5f - (Player->GetScore() * 0.00001f);

	if (percentChance < 0) percentChance = 0;

	float angle = GetAngleFromWorldVectorZ(Player->Position) +
		M.GetRandomFloat(-percentChance, percentChance);
	RotationZ = angle -Parents->at(0)->RotationZ;
	float shotSpeed = 200;

	Vector3 offset = Vector3Add(GetVelocityFromAngleZ(Radius), GetWorldPosition());
	Shots.at(shotNumber)->Spawn(offset, GetVelocityFromAngleZ(angle, shotSpeed), 4.75f);
}
