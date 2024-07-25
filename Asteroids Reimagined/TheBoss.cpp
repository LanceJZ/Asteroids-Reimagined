#include "TheBoss.h"

TheBoss::TheBoss()
{
	MissileFireTimerID = Managers.EM.AddTimer(1.25f);
	MineDropTimerID = Managers.EM.AddTimer(1.5f);
	FireTimerID = Managers.EM.AddTimer();

	Managers.EM.AddLineModel(LeftSpineMount = DBG_NEW LineModel());
	Managers.EM.AddLineModel(RightSpineMount = DBG_NEW LineModel());
	Managers.EM.AddLineModel(Shield = DBG_NEW LineModel());
	Managers.EM.AddEntity(FireShotAtPlayerArea = DBG_NEW Entity());

	for (int i = 0; i < 5; i++)
	{
		Managers.EM.AddLineModel(Turrets[i] = DBG_NEW TheBossTurret());
	}
}

TheBoss::~TheBoss()
{
}

void TheBoss::SetPlayer(ThePlayer* player)
{
	Player = player;

	for (int i = 0; i < 5; i++)
	{
		Turrets[i]->SetPlayer(player);
	}
}

void TheBoss::SetShieldModel(LineModelPoints model)
{
	Shield->SetModel(model);
}

void TheBoss::SetTurretModel(LineModelPoints model)
{
	for (int i = 0; i < 5; i++)
	{
		Turrets[i]->SetModel(model);
	}
}

void TheBoss::SetLeftSpineMountModel(LineModelPoints model)
{
	LeftSpineMount->SetModel(model);
}

void TheBoss::SetRightSpineMountModel(LineModelPoints model)
{
	RightSpineMount->SetModel(model);
}

void TheBoss::SetMissileModel(LineModelPoints model)
{

	MissileModel = model;
}

void TheBoss::SetShotModel(LineModelPoints model)
{

	ShotModel = model;

	for (int i = 0; i < 5; i++)
	{
		Turrets[i]->SetShotModel(model);
	}
}

void TheBoss::SetMineModel(LineModelPoints model)
{

	MineModel = model;
}

bool TheBoss::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	FireShotAtPlayerArea->Enabled = false;

	return true;
}

bool TheBoss::BeginRun()
{
	LineModel::BeginRun();

	Shield->SetParent(*this);
	FireShotAtPlayerArea->SetParent(*this);
	FireShotAtPlayerArea->X(370.0f);
	FireShotAtPlayerArea->Radius = 250.0f;
	FireShotAtPlayerArea->EntityOnly = true;

	for (int i = 0; i < 5; i++)
	{
		Turrets[i]->SetParent(*this);
	}

	Turrets[0]->Position = { 0.0f, 37.0f * 2, 0.0f };
	Turrets[1]->Position = { 0.0f, -37.0f * 2, 0.0f };
	Turrets[2]->Position = { 31.0f * 2, 0.0f, 0.0f };
	Turrets[3]->Position = { -29.0f * 2, 25.0f * 2, 0.0f };
	Turrets[4]->Position = { -29.0f * 2, -25.0f * 2, 0.0f };

	LeftSpineMount->SetParent(*this);
	RightSpineMount->SetParent(*this);

	LeftSpineMount->Position = { 19.0f * 2.0f, 30.0f * 2.0f, 0.0f };
	RightSpineMount->Position = { 19.0f * 2.0f, -30.0f * 2.0f, 0.0f };

	float edge = 0.666f;
	float upper = WindowHeight * edge;
	float lower = -WindowHeight * edge;
	float left = -WindowWidth * edge;
	float right = WindowWidth * edge;

	Path.push_back({ left, upper, 0.0f });
	Path.push_back({ right, upper, 0.0f });
	Path.push_back({ right, lower, 0.0f });
	Path.push_back({ left, lower, 0.0f });

	FireTimerSetting = GetRandomFloat(0.75f, 1.5f);

	Reset();

	return false;
}

void TheBoss::Update(float deltaTime)
{
	LineModel::Update(deltaTime);


	if (Shield->Enabled)
	{
		if (ShieldPower <= 0)
		{
			Shield->Enabled = false;
		}
	}

	FireShotAtPlayerArea->Enabled = Enabled;

	HeadToNextWaypoint();
	ReachedWaypoint();
	CheckCollisions();
}

void TheBoss::Draw3D()
{
	LineModel::Draw3D();
}

void TheBoss::Reset()
{
	Destroy();

	Shield->Enabled = false;
	LeftSpineMount->Enabled = false;
	RightSpineMount->Enabled = false;

	for (const auto& shot : Shots)
	{
		shot->Destroy();
	}

	for (const auto& missile : Missiles)
	{
		missile->Destroy();
	}

	for (const auto& mine : Mines)
	{
		mine->Destroy();
	}
}

void TheBoss::Spawn(Vector3 position, float rotation)
{
	Entity::Spawn(position);

	RotationZ = rotation;
	HideCollision = true;
	Shield->Enabled = true;
	LeftSpineMount->Enabled = true;
	RightSpineMount->Enabled = true;
	Shield->ShowCollision = true;
	ShieldPower = 100;

	for (const auto& turret : Turrets)
	{
		turret->Spawn();
	}
}

void TheBoss::Hit()
{
	Entity::Hit();

}

void TheBoss::Destroy()
{
	Entity::Destroy();

	FireShotAtPlayerArea->Enabled = false;

	for (const auto& turret : Turrets)
	{
		turret->Destroy();
	}
}

void TheBoss::HeadToNextWaypoint()
{
	SetRotateVelocity(Path[NextWaypoint], 0.20, 20);
}

void TheBoss::ReachedWaypoint()
{
	if (CirclesIntersect(Path[NextWaypoint], 10.0f))
	{
		NextWaypoint = GetRandomValue(0, Path.size() - 1);
	}
}

void TheBoss::CheckCollisions()
{
	if (Player->CirclesIntersect(FireShotAtPlayerArea->GetWorldPosition(),
		FireShotAtPlayerArea->Radius) && Player->Enabled)
	{
		if (Managers.EM.TimerElapsed(FireTimerID))
		{
			FireShots();
		}
	}

	for (const auto& shot : Player->Shots)
	{
		if (!shot->Enabled) continue;

		if (Shield->Enabled)
		{
			if (shot->CirclesIntersect(Shield->GetWorldPosition(), Shield->Radius))
			{
				shot->Destroy();
				ShieldPower -= 10;

				Shield->Alpha = ShieldPower * 2.55f;

				if (ShieldPower <= 0)
				{
					Shield->Enabled = false;
				}
			}
		}
		else
		{
			bool turretsAllDead = true;

			for (const auto& turret : Turrets)
			{
				if (!turret->Enabled) continue;

				if (shot->CirclesIntersect(turret->GetWorldPosition(), turret->Radius))
				{
					shot->Destroy();
					turret->Hit();

				}

				turretsAllDead = false;
			}

			if (turretsAllDead)
			{
				if (shot->CirclesIntersect(*this))
				{
					shot->Destroy();

					HitPoints -= 10;

					unsigned char color = (unsigned char)(HitPoints * 2.55);

					ModelColor = { 255, color, color, 255 };

					if (HitPoints <= 0)
					{
						Player->ScoreUpdate(5550);
						Hit();
					}
				}
			}
		}

	}


}

void TheBoss::FireShots()
{
	FireTimerSetting = GetRandomFloat(0.25f, 0.75f);
	Managers.EM.ResetTimer(FireTimerID, FireTimerSetting);

	if (!Player->Enabled) return;

	//if (!Player->GameOver) PlaySound(FireSound);

	bool spawnNewShotL = true;
	size_t shotNumberL = Shots.size();

	for (size_t check = 0; check < shotNumberL; check++)
	{
		if (!Shots.at(check)->Enabled)
		{
			spawnNewShotL = false;
			shotNumberL = check;
			break;
		}
	}

	if (spawnNewShotL)
	{
		Shots.push_back(DBG_NEW Shot());
		Managers.EM.AddLineModel(Shots.back(), ShotModel);
		Shots.back()->SetModel(ShotModel);
		Shots.back()->Initialize(TheUtilities);
		Shots.back()->BeginRun();
	}

	float shotSpeed = 200;
	Vector3 offset = Vector3Add(VelocityFromAngleZ(LeftSpineMount->Radius),
		LeftSpineMount->GetWorldPosition());
	Shots.at(shotNumberL)->Spawn(offset,
		GetVelocityFromAngleZ(RotationZ, shotSpeed), 4.75f);

	bool spawnNewShotR = true;
	size_t shotNumberR = Shots.size();

	for (size_t check = 0; check < shotNumberR; check++)
	{
		if (!Shots.at(check)->Enabled)
		{
			spawnNewShotR = false;
			shotNumberR = check;
			break;
		}
	}

	if (spawnNewShotR)
	{
		Shots.push_back(DBG_NEW Shot());
		Managers.EM.AddLineModel(Shots.back(), ShotModel);
		Shots.back()->SetModel(ShotModel);
		Shots.back()->Initialize(TheUtilities);
		Shots.back()->BeginRun();
	}

	offset = Vector3Add(VelocityFromAngleZ(RightSpineMount->Radius),
		RightSpineMount->GetWorldPosition());
	Shots.at(shotNumberR)->Spawn(offset,
		GetVelocityFromAngleZ(RotationZ, shotSpeed), 4.75f);
}
