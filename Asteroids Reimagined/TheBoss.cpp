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

void TheBoss::SetParticleManager(ParticleManager* particleManager)
{
	Particles = particleManager;

	for (const auto& turret : Turrets)
	{
		turret->SetParticleManager(particleManager);
	}
}

void TheBoss::SetShieldModel(LineModelPoints model)
{
	Shield->SetModel(model);
}

void TheBoss::SetTurretModel(LineModelPoints model)
{
	for (const auto& turret : Turrets)
	{
		turret->SetModel(model);
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

void TheBoss::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
}

void TheBoss::SetShieldHitSound(Sound sound)
{
	ShieldHitSound = sound;
}

void TheBoss::SetShieldDownSound(Sound sound)
{
	ShieldDownSound = sound;
}

void TheBoss::SetHitSound(Sound sound)
{
	HitSound = sound;
}

void TheBoss::SetTurretFireSound(Sound sound)
{
	TurretFireSound = sound;

	for (const auto& turret : Turrets)
	{
		turret->SetFireSound(TurretFireSound);
	}
}

void TheBoss::SetTurretExplodeSound(Sound sound)
{
	TurretExplodeSound = sound;

	for (const auto& turret : Turrets)
	{
		turret->SetExplodeSound(TurretExplodeSound);
	}
}

void TheBoss::SetSpineFireSound(Sound sound)
{
	SpineFireSound = sound;
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
	FireShotAtPlayerArea->Radius = 300.0f;
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
	HitPoints = 100;
	ModelColor = { 255, 255, 255, 255 };
	Shield->Alpha = 255;

	for (const auto& turret : Turrets)
	{
		turret->Spawn();
	}
}

void TheBoss::Hit()
{
	Entity::Hit();

	if (!Player->GameOver) PlaySound(ExplodeSound);

	Particles->SpawnLineParticles(Position,
		Vector3Multiply(Velocity, {0.25f}),
		20, 100, 20, 2.0f, WHITE);

	Destroy();
}

void TheBoss::Destroy()
{
	Entity::Destroy();

	FireShotAtPlayerArea->Enabled = false;

	for (const auto& turret : Turrets)
	{
		turret->Destroy();
	}

	LeftSpineMount->Enabled = false;
	RightSpineMount->Enabled = false;
}

void TheBoss::Hit(Entity* entity, int damage)
{
	if (Shield->Enabled)
	{
		ShieldHit(damage);
		entity->Destroy();
	}
	else
	{
		ShieldDown(entity, damage);
	}
}

void TheBoss::HeadToNextWaypoint()
{
	SetRotateVelocity(Path[NextWaypoint], 0.20f, 20.0f);
}

void TheBoss::ReachedWaypoint()
{
	if (CirclesIntersect(Path[NextWaypoint], 10.0f))
	{
		NextWaypoint = GetRandomValue(0, (int)Path.size() - 1);
	}
}

void TheBoss::CheckCollisions()
{
	if (Player->Enabled)
	{
		if (Player->CirclesIntersect(FireShotAtPlayerArea->GetWorldPosition(),
			FireShotAtPlayerArea->Radius))
		{
			if (Managers.EM.TimerElapsed(FireTimerID))
			{
				FireShots();
			}
		}

		for (const auto& turret : Turrets)
		{
			turret->CheckCollisions();
		}

		for (const auto& shot : Shots)
		{
			if (shot->CirclesIntersect(*Player))
			{
				Player->Hit(shot->Position, shot->Velocity);
				break;
			}
		}
	}

	for (const auto& shot : Player->Shots)
	{
		if (!shot->Enabled) continue;

		if (shot->CirclesIntersect(Shield->GetWorldPosition(), Shield->Radius))
		{
			Hit(shot, 2);
		}
	}

	for (const auto& shot : Player->BigShots)
	{
		if (!shot->Enabled) continue;

		if (shot->CirclesIntersect(Shield->GetWorldPosition(), Shield->Radius))
		{
			Hit(shot, 10);
		}
	}

	for (const auto& shot : Player->DoubleShots)
	{
		if (!shot->Enabled) continue;

		if (shot->CirclesIntersect(Shield->GetWorldPosition(), Shield->Radius))
		{
			Hit(shot, 5);
		}
	}

	for (const auto& shot : Player->PlasmaShots)
	{
		if (!shot->Enabled) continue;

		if (shot->CirclesIntersect(Shield->GetWorldPosition(), Shield->Radius))
		{
			Hit(shot, 50);
		}
	}
}

void TheBoss::FireShots()
{
	FireTimerSetting = GetRandomFloat(0.25f, 0.75f);
	Managers.EM.ResetTimer(FireTimerID, FireTimerSetting);

	if (!Player->Enabled) return;

	if (!Player->GameOver) PlaySound(SpineFireSound);

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
	Vector3 offset = Vector3Add(GetVelocityFromAngleZ(LeftSpineMount->Radius),
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

	offset = Vector3Add(GetVelocityFromAngleZ(RightSpineMount->Radius),
		RightSpineMount->GetWorldPosition());
	Shots.at(shotNumberR)->Spawn(offset,
		GetVelocityFromAngleZ(RotationZ, shotSpeed), 4.75f);
}

void TheBoss::ShieldHit(int damage)
{
	ShieldPower -= damage;
	PlaySound(ShieldHitSound);
	Shield->Alpha = ShieldPower * 2.55f;

	if (ShieldPower <= 0)
	{
		Shield->Enabled = false;
		PlaySound(ShieldDownSound);
		Player->ScoreUpdate(100);
	}
}

void TheBoss::ShieldDown(Entity* shot, int damage)
{
	bool turretsAllDead = true;

	for (const auto& turret : Turrets)
	{
		if (!turret->Enabled) continue;

		if (shot->CirclesIntersect(turret->GetWorldPosition(), turret->Radius))
		{
			shot->Destroy();
			turret->Hit();
			Player->ScoreUpdate(500);

		}

		turretsAllDead = false;
	}

	if (turretsAllDead)
	{
		if (shot->CirclesIntersect(*this))
		{
			shot->Destroy();

			PlaySound(HitSound);

			HitPoints -= damage;

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
