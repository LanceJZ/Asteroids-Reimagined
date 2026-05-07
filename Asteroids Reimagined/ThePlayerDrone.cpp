#include "ThePlayerDrone.h"

ThePlayerDrone::ThePlayerDrone()
{
	FireRateTimerID = EM.AddTimer(0.125f);
	
	MagazineSize = 10;

}

ThePlayerDrone::~ThePlayerDrone()
{
}

void ThePlayerDrone::SetPlayer(ThePlayer* player)
{
	Player = player;
}

bool ThePlayerDrone::Initialize()
{
	ThePlayerBase::Initialize();

	return false;
}

bool ThePlayerDrone::BeginRun()
{
	ThePlayerBase::BeginRun();

	Enabled = false;

	return true;
}

void ThePlayerDrone::Update(float deltaTime)
{
	ThePlayerBase::Update(deltaTime);

}

void ThePlayerDrone::FixedUpdate(float deltaTime)
{
	ThePlayerBase::FixedUpdate(deltaTime);

	if (EM.TimerElapsed(FireRateTimerID))
	{
		FireShot();
	}
}

void ThePlayerDrone::Draw3D()
{
	ThePlayerBase::Draw3D();
}

void ThePlayerDrone::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void ThePlayerDrone::Destroy()
{
	Entity::Destroy();

}

void ThePlayerDrone::FireShot()
{
	float angle = RotationZ;
	float shotSpeed = 525;
	bool noEnemies = true;

	for (const auto &enemy : Enemies)
	{
		if (enemy->Enabled)
		{
			noEnemies = false;
			angle = GetAngleFromVectorsZ(Position, enemy->Position);
			break;
		}
	}

	if (noEnemies) angle = SM.AimedShotAtNearbyRock(Position);

	EM.ResetTimer(FireRateTimerID);

	bool spawnNew = true;
	size_t spawnNumber = Shots.size();

	for (size_t check = 0; check < spawnNumber; check++)
	{
		if (!Shots[check]->Enabled)
		{
			spawnNew = false;
			spawnNumber = check;
			break;
		}
	}

	if (spawnNew)
	{
		Shots.push_back(DBG_NEW Shot());
		EM.AddLineModel(Shots[spawnNumber], ShotModel);
		Shots[spawnNumber]->BeginRun();
	}

	//if (!Player->GameOver) PlaySound(FireSound);

	RotationZ = angle;

	Vector3 position = Vector3Add(GetVelocityFromAngleZ(Radius), Position);
	Shots[spawnNumber]->Spawn(position, GetVelocityFromAngleZ(shotSpeed), 2.5f);
}