#include "TheBoss.h"

TheBoss::TheBoss()
{
	MissileFireTimerID = Managers.EM.AddTimer(1.25f);
	MineDropTimerID = Managers.EM.AddTimer(1.5f);

	Managers.EM.AddEntity(Shield = DBG_NEW LineModel());

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

void TheBoss::SetMissileModel(LineModelPoints model)
{

	MissileModel = model;
}

void TheBoss::SetShotModel(LineModelPoints model)
{

	ShotModel = model;
}

void TheBoss::SetMineModel(LineModelPoints model)
{

	MineModel = model;
}

bool TheBoss::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	ActualShipRadius = Radius;

	float upper = WindowHeight * 0.75f;
	float lower = -WindowHeight * 0.75f;
	float left = -WindowWidth * 0.75f;
	float right = WindowWidth * 0.75f;

	Path.push_back({ upper, left, 0.0f });
	Path.push_back({ upper, right, 0.0f });
	Path.push_back({ lower, right, 0.0f });
	Path.push_back({ lower, left, 0.0f });

	return false;
}

bool TheBoss::BeginRun()
{
	LineModel::BeginRun();

	Shield->SetParent(*this);

	for (int i = 0; i < 5; i++)
	{
		Turrets[i]->SetParent(*this);
	}

	Turrets[0]->Position = { 0.0f, 37.0f * 2, 0.0f };
	Turrets[1]->Position = { 0.0f, -37.0f * 2, 0.0f };
	Turrets[2]->Position = { 31.0f * 2, 0.0f, 0.0f };
	Turrets[3]->Position = { -29.0f * 2, 25.0f * 2, 0.0f };
	Turrets[4]->Position = { -29.0f * 2, -25.0f * 2, 0.0f };

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
			Radius = ActualShipRadius;
		}
	}
}

void TheBoss::Draw3D()
{
	LineModel::Draw3D();
}

void TheBoss::Reset()
{
	Destroy();

	Shield->Enabled = false;

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
	Radius = Shield->Radius;
	Shield->Enabled = true;
	ShieldPower = 100;

	for (const auto& turret : Turrets)
	{
		turret->Spawn();
	}
}

void TheBoss::Destroy()
{
	Entity::Destroy();

	for (const auto& turret : Turrets)
	{
		turret->Destroy();
	}
}
