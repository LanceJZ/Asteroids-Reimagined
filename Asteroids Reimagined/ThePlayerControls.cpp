#include "ThePlayerControls.h"

ThePlayerControls::ThePlayerControls()
{
	EM.AddLineModel(Flame = DBG_NEW LineModel());
	EM.AddLineModel(Shield = DBG_NEW LineModel());
	EM.AddLineModel(Turret = DBG_NEW LineModel());

	FireRateTimerID = EM.AddTimer(0.125f);
	TurretCooldownTimerID = EM.AddTimer(2.0f);
	TurretHeatTimerID = EM.AddTimer(0.15f);
	PowerupTimerID = EM.AddTimer();
	FlameColorTimerID = EM.AddTimer(0.15f);

	MagazineSize = 16;

	for (int i = 0; i < MagazineSize; i++)
	{
		Shots.push_back(DBG_NEW Shot());
		EM.AddLineModel(Shots.back());
	}
}

ThePlayerControls::~ThePlayerControls()
{
}

void ThePlayerControls::SetTurretModel(std::vector<Vector3> model)
{
	Turret->SetModel(model);
}

void ThePlayerControls::SetShotModel(std::vector<Vector3> model)
{
	ShotModel = model;

	for (const auto& shot : Shots)
	{
		shot->SetModel(model);
	}
}

void ThePlayerControls::SetFlameModel(std::vector<Vector3> model)
{
	Flame->SetModel(model);
}

void ThePlayerControls::SetShieldModel(std::vector<Vector3> model)
{
	Shield->SetModel(model);
}

void ThePlayerControls::SetBigShotModel(std::vector<Vector3> model)
{
	BigShotModel = model;
}

void ThePlayerControls::SetMineModel(std::vector<Vector3> model)
{
	MineModel = model;
}

void ThePlayerControls::SetFireSound(Sound sound)
{
	FireSound = sound;
}

void ThePlayerControls::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
}

void ThePlayerControls::SetShieldOnSound(Sound sound)
{
	ShieldOnSound = sound;
}

void ThePlayerControls::SetShieldHitSound(Sound sound)
{
	ShieldHitSound = sound;
}

void ThePlayerControls::SetThrustSound(Sound sound)
{
	ThrustSound = sound;
}

void ThePlayerControls::SetSpawnSound(Sound sound)
{
	SpawnSound = sound;
}

void ThePlayerControls::SetBigShotSound(Sound sound)
{
	BigShotSound = sound;
}

void ThePlayerControls::SetDoubleShotSound(Sound sound)
{
	DoubleShotSound = sound;
}

void ThePlayerControls::SetPlasmaShotSound(Sound sound)
{
	PlasmaShotSound = sound;
}

void ThePlayerControls::SetMineDropSound(Sound sound)
{
	MineDropSound = sound;
}

void ThePlayerControls::SetMineExplodeSound(Sound sound)
{
	MineExplodeSound = sound;
}

bool ThePlayerControls::Initialize()
{
	LineModel::Initialize();

	Flame->Enabled = false;
	Shield->Enabled = false;
	Turret->Enabled = false;

	GameOver = true;
	Enabled = false;

	for (const auto& shot : Shots)
	{
		shot->Initialize();
		shot->Enabled = false;
	}

	return true;
}

bool ThePlayerControls::BeginRun()
{
	LineModel::BeginRun();

	Flame->SetParent(*this);
	Turret->SetParent(*this);
	Shield->SetParent(*this);
	Shield->ShowCollision = true;
	Turret->SetParent(*this);

	for (const auto& shot : Shots)
	{
		shot->BeginRun();
	}

	return true;
}

void ThePlayerControls::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void ThePlayerControls::FixedUpdate(float deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	ShieldPowerDrain(deltaTime);
	CheckScreenEdge();
}

void ThePlayerControls::Draw3D()
{
	LineModel::Draw3D();
}

void ThePlayerControls::Hit()
{
	LineModel::Hit();

	PlaySound(ExplodeSound);
	Particles.SpawnLineDots(Position, Vector3Multiply(Velocity, { 0.15f }),
		Radius * 0.25f, 30.0f, 20, 3.5f, WHITE);
	Particles.SpawnLineModelExplosion(GetLineModel(), Position, Velocity,
		RotationZ, 10.0f, 4.0f, ModelColor);
	Particles.SpawnLineModelExplosion(Turret->GetLineModel(), Position, Velocity,
		Turret->RotationZ, 15.0f, 3.0f, Turret->ModelColor);
	Acceleration = { 0 };
	Velocity = { 0 };

	RotateStop();

}

void ThePlayerControls::ShieldHit(Vector3 location, Vector3 velocity)
{
}

void ThePlayerControls::Spawn()
{
}

void ThePlayerControls::NewGame()
{
}

void ThePlayerControls::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void ThePlayerControls::ShieldPowerUp()
{
	ShieldOverCharge = true;
	ShieldPower += 200;
	Shield->ModelColor = PINK;
	Shield->Alpha = 255.0f;
	if (!PoweredUp) ModelColor = PINK;
}

void ThePlayerControls::GunPowerUp()
{
}

void ThePlayerControls::FullPowerUp()
{
	EM.ResetTimer(PowerupTimerID,
	EM.GetTimerAmount(PowerupTimerID) + PowerUpTimerAmount);
	ModelColor = RED;
	PoweredUp = true;
	PoweredUpRundown = false;
	TurretOverheat = false;
	Shield->Alpha = 255.0f;
}

void ThePlayerControls::BigShotPowerUp()
{
}

void ThePlayerControls::DoubleShotPowerUp()
{
}

void ThePlayerControls::MinePowerUp()
{
}

void ThePlayerControls::PlasmaShotPowerUp()
{
}

void ThePlayerControls::Destroy()
{
	Entity::Destroy();

}

void ThePlayerControls::PointTurret(Vector3 mouseLocation)
{
	Turret->RotationZ = GetAngleFromVectorZ(mouseLocation) - RotationZ;
}

void ThePlayerControls::FireTurret()
{
}

void ThePlayerControls::TurretTimers()
{
	if (TurretOverheat)
	{
		if (EM.TimerElapsed(TurretCooldownTimerID))
		{
			TurretOverheatCooldown = true;
			TurretOverheat = false;
			TurretHeat = 0;
		}
	}
	else
	{
		if (EM.TimerElapsed(TurretHeatTimerID))
		{
			EM.ResetTimer(TurretHeatTimerID);
			TurretOverheatRundown = true;

			if (TurretHeat > 2)	TurretHeat -= 2;
			if (TurretHeat >= 1) TurretHeat -= 1;
		}
	}
}

void ThePlayerControls::FireSecondary()
{
}

void ThePlayerControls::FireBigShot()
{
}

void ThePlayerControls::FireDoubleShot()
{
}

void ThePlayerControls::DropHomingMine()
{
}

void ThePlayerControls::FirePlasmaShot()
{
}

void ThePlayerControls::RotateShip(float amount)
{
	RotationVelocityZ = (amount * 5.5f);
}

void ThePlayerControls::RotateStop()
{
	RotationVelocityZ = 0.0f;
}

void ThePlayerControls::ThrustOn(float amount)
{
	if (!Enabled || GetBeenHit()) return;

	SetAccelerationToMaxAtRotation((amount * 50.25f), 350.0f);
	Flame->Enabled = true;

	if (EM.TimerElapsed(FlameColorTimerID))
	{
		EM.ResetTimer(FlameColorTimerID, M.GetRandomFloat(0.025f, 0.25f));

		Color color = {(unsigned char)GetRandomValue(100, 255),
			(unsigned char)GetRandomValue(60, 105),
			(unsigned char)GetRandomValue(0, 255), 255};

		Flame->ModelColor = color;
	}

	if (!IsSoundPlaying(ThrustSound)) PlaySound(ThrustSound);
}

void ThePlayerControls::ThrustOff()
{
	StopSound(ThrustSound);
	SetAccelerationToZero(0.45f);
	Flame->Enabled = false;
}

void ThePlayerControls::ShieldOn()
{
	if (ShieldPower > 0.0f)
	{
		Shield->Enabled = true;

		if (!IsSoundPlaying(ShieldOnSound)) PlaySound(ShieldOnSound);

		if (ShieldOverCharge)
		{
			if (ShieldPower < 100.0f)
			{
				Shield->ModelColor = WHITE;
				ModelColor = WHITE;
				ShieldOverCharge = false;
			}
		}
		else
		{
			Shield->Alpha = ShieldPower * 2.55f;
		}
	}
	else
	{
		ShieldOff();
	}
}

void ThePlayerControls::ShieldOff()
{
}

void ThePlayerControls::ShieldPowerDrain(float deltaTime)
{
	if (Shield->Enabled)
	{
		ShieldPower -= ShieldDrainRate * deltaTime;

		if (ShieldPower < 0.0f)	ShieldPower = 0.0f;
	}
	else
	{
		if (ShieldPower < 100.0f) ShieldPower += ShieldRechargeRate * deltaTime;
	}
}

void ThePlayerControls::WeHaveThePower()
{
	if (ShieldPower < 100.0f) ShieldPower = 100.0f;

	if (TurretHeat > 0) TurretHeat = 0;

	ShieldOn();
}

void ThePlayerControls::SwitchSecondaryWeapon(SecondaryWeaponType type)
{
}

void ThePlayerControls::IsSecondaryWeaponSwitched(float next)
{
}
