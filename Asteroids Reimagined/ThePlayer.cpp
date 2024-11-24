#include "ThePlayer.h"

ThePlayer::ThePlayer()
{
	Managers.EM.AddLineModel(Flame = DBG_NEW LineModel());
	Managers.EM.AddLineModel(Shield = DBG_NEW LineModel());
	Managers.EM.AddLineModel(Turret = DBG_NEW LineModel());
	Managers.EM.AddLineModel(Crosshair = DBG_NEW LineModel());
	Managers.EM.AddLineModel(TurretHeatMeter = DBG_NEW LineModel());
	Managers.EM.AddLineModel(WeaponTypeIconDoubleLeft = DBG_NEW LineModel());
	Managers.EM.AddLineModel(WeaponTypeIconDoubleRight = DBG_NEW LineModel());
	Managers.EM.AddLineModel(WeaponTypeIconBig = DBG_NEW LineModel());
	Managers.EM.AddLineModel(WeaponTypeIconMine = DBG_NEW LineModel());
	Managers.EM.AddLineModel(WeaponTypeIconPlasma = DBG_NEW LineModel());

	Managers.EM.AddOnScreenText(Score = DBG_NEW TheScore());

	FireRateTimerID = Managers.EM.AddTimer(0.125f);
	TurretCooldownTimerID = Managers.EM.AddTimer(2.0f);
	TurretHeatTimerID = Managers.EM.AddTimer(0.15f);
	PowerupTimerID = Managers.EM.AddTimer();
	PowerupRundownTimerID = Managers.EM.AddTimer(2.0f);
	PowerUpBlinkTimerID = Managers.EM.AddTimer(0.25f);
	FlameColorTimerID = Managers.EM.AddTimer(0.15f);

	for (int i = 0; i < MagazineSize; i++)
	{
		Shots.push_back(DBG_NEW Shot());
		Managers.EM.AddLineModel(Shots.back());
	}
}

ThePlayer::~ThePlayer()
{
}

void ThePlayer::SetTurretModel(LineModelPoints model)
{
	Turret->SetModel(model);
}

void ThePlayer::SetShotModel(LineModelPoints model)
{
	for (auto& shot : Shots)
	{
		shot->SetModel(model);
		shot->BeginRun();
	}

	ShotModel = model;

	WeaponTypeIconDoubleLeft->SetModel(model);
	WeaponTypeIconDoubleRight->SetModel(model);
}

void ThePlayer::SetFlameModel(LineModelPoints model)
{
	Flame->SetModel(model);
}

void ThePlayer::SetShieldModel(LineModelPoints model)
{
	Shield->SetModel(model);
}

void ThePlayer::SetTurretHeatModel(LineModelPoints model)
{
	TurretHeatMeter->SetModel(model);
}

void ThePlayer::SetBigShotModel(LineModelPoints model)
{
	BigShotModel = model;
	WeaponTypeIconBig->SetModel(model);
	WeaponTypeIconPlasma->SetModel(model);
}

void ThePlayer::SetMineModel(LineModelPoints model)
{
	MineModel = model;
	WeaponTypeIconMine->SetModel(model);
}

void ThePlayer::SetFireSound(Sound sound)
{
	FireSound = sound;
	SetSoundVolume(FireSound, 0.25f);
}

void ThePlayer::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
	SetSoundVolume(ExplodeSound, 0.95f);
}

void ThePlayer::SetShieldOnSound(Sound sound)
{
	ShieldOnSound = sound;
}

void ThePlayer::SetShieldHitSound(Sound sound)
{
	ShieldHitSound = sound;
}

void ThePlayer::SetThrustSound(Sound sound)
{
	ThrustSound = sound;
	SetSoundVolume(ThrustSound, 0.5f);
}

void ThePlayer::SetSpawnSound(Sound sound)
{
	SpawnSound = sound;
}

void ThePlayer::SetBonusSound(Sound sound)
{
	BonusSound = sound;
}

void ThePlayer::SetBigShotSound(Sound sound)
{
	BigShotSound = sound;
	SetSoundVolume(BigShotSound, 0.25f);
}

void ThePlayer::SetDoubleShotSound(Sound sound)
{
	DoubleShotSound = sound;
	SetSoundVolume(DoubleShotSound, 0.5f);
}

void ThePlayer::SetPlasmaShotSound(Sound sound)
{
	PlasmaShotSound = sound;
}

void ThePlayer::SetMineDropSound(Sound sound)
{
	MineDropSound = sound;
}

void ThePlayer::SetMineExplodeSound(Sound sound)
{
	MineExplodeSound = sound;
}

void ThePlayer::SetParticleManager(ParticleManager* particleManager)
{
	Particles = particleManager;
}

void ThePlayer::SetCrosshairModel(LineModelPoints model)
{
	Crosshair->SetModel(model);
	Crosshair->Radius = 0.0f;
}

bool ThePlayer::Initialize(Utilities* utilities)
{
	LineModel::Initialize(TheUtilities);
	//Scale = 35.0f;

	Flame->Enabled = false;
	Shield->Enabled = false;
	Turret->Enabled = false;
	Crosshair->Enabled = false;
	WeaponTypeIconBig->Enabled = false;
	WeaponTypeIconDoubleLeft->Enabled = false;
	WeaponTypeIconDoubleRight->Enabled = false;
	WeaponTypeIconPlasma->Enabled = false;
	WeaponTypeIconMine->Enabled = false;

	PowerUpTimerAmount = 8.0f;

	Flame->ModelColor = { 255, 140, 0, 255 };

	GameOver = true;
	Enabled = false;

	return false;
}

bool ThePlayer::BeginRun()
{
	Flame->SetParent(*this);
	Shield->SetParent(*this);

	Turret->X(-9.0f);
	Turret->SetParent(*this);

	TurretHeatMeter->Y(-25.0f);
	TurretHeatMeter->SetParent(*this);
	TurretHeatMeter->Enabled = false;
	TurretHeatMeter->Radius = 0.0f;
	TurretHeatMeter->RotationLocked = true;

	WeaponTypeIconBig->SetParent(*this);
	WeaponTypeIconDoubleLeft->SetParent(*this);
	WeaponTypeIconDoubleRight->SetParent(*this);
	WeaponTypeIconPlasma->SetParent(*this);
	WeaponTypeIconMine->SetParent(*this);

	WeaponTypeIconBig->RotationLocked = true;
	WeaponTypeIconDoubleLeft->RotationLocked = true;
	WeaponTypeIconDoubleRight->RotationLocked = true;
	WeaponTypeIconPlasma->RotationLocked = true;
	WeaponTypeIconMine->RotationLocked = true;

	float iconX = - 15.0f;

	WeaponTypeIconBig->X(iconX);
	WeaponTypeIconDoubleLeft->X(iconX - 2.0f);
	WeaponTypeIconDoubleRight->X(iconX + 2.0f);
	WeaponTypeIconPlasma->X(iconX - 2.0f);
	WeaponTypeIconMine->X(iconX + 2.0f);

	float iconY = 30.0f;

	WeaponTypeIconBig->Y(iconY);
	WeaponTypeIconDoubleLeft->Y(iconY);
	WeaponTypeIconDoubleRight->Y(iconY);
	WeaponTypeIconPlasma->Y(iconY);
	WeaponTypeIconMine->Y(iconY);

	WeaponTypeIconBig->Scale = 0.5f;
	WeaponTypeIconMine->Scale = 0.5f;
	WeaponTypeIconDoubleLeft->Scale = 0.5f;
	WeaponTypeIconDoubleRight->Scale = 0.5f;

	return false;
}

void ThePlayer::Input()
{
	LineModel::Input();

	if (GameOver || Paused || !Enabled || GetBeenHit()) return;

	if (IsGamepadAvailable(0) && Enabled)
	{
		Gamepad();
		Crosshair->Enabled = false;
	}
	else if (Enabled)
	{
		Keyboard();

		if (AltMouseMode) AltMouse();
		else Mouse();
	}
}

void ThePlayer::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	WeaponPlasmaIconUpdate(deltaTime);
	CrosshairUpdate();
	TurretTimers();

	if (PoweredUp) WeHaveThePower();
}

void ThePlayer::FixedUpdate(float deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	CheckScreenEdge();
	ShieldPowerDrain(deltaTime);
}

void ThePlayer::Draw3D()
{
	LineModel::Draw3D();

}

void ThePlayer::Hit(Vector3 location, Vector3 velocity)
{
	if (Shield->Enabled)
	{
		ShieldHit(location, velocity);
	}
	else
	{
		Entity::Hit();

		PlaySound(ExplodeSound);
		Particles->SpawnLineParticles(Position, Vector3Multiply(Velocity, { 0.25f }),
			Radius * 0.25f, 30.0f, 40, 3.0f, WHITE);
		Acceleration = { 0 };
		Velocity = { 0 };
		Lives--;
		Turret->Enabled = false;
		Flame->Enabled = false;
		Shield->Enabled = false;
		Crosshair->Enabled = false;
		TurretHeatMeter->Enabled = false;
		WeaponTypeIconBig->Enabled = false;
		WeaponTypeIconDoubleLeft->Enabled = false;
		WeaponTypeIconDoubleRight->Enabled = false;
		WeaponTypeIconPlasma->Enabled = false;
		WeaponTypeIconMine->Enabled = false;
		RotateStop();

		for (const auto& model : AmmoMeterModels)
		{
			model->Enabled = false;
		}

		if (Lives < 0)
		{
			GameOver = true;

			for (const auto& mine : Mines)
			{
				mine->GameOver = true;
			}
		}
	}
}

void ThePlayer::ScoreUpdate(int addToScore)
{
	Score->UpdateScore(addToScore);

	if (Score->GetScore() > NextNewLifeScore)
	{
		NextNewLifeScore += ((10000 * (Wave + 1)) * 4);
		Lives++;
		NewLife = true;
		PlaySound(BonusSound);
	}
}

int ThePlayer::GetScore()
{
	return Score->GetScore();
}

void ThePlayer::SetHighScore(int highScore)
{
	Score->SetHighScore(highScore);
}

void ThePlayer::Spawn()
{
	Entity::Spawn({ 0, 0, 0 });

	Velocity = { 0, 0, 0 };

	Enabled = true;
	Turret->Enabled = true;
	TurretHeatMeter->Enabled = true;
	ShieldPower = 100.0f;
	TurretHeatMeter->Scale = 0.0f;
	TurretOverHeat = false;
	TurretHeat = 0;
	PoweredUp = false;
	PoweredUpRundown = false;
	ModelColor = WHITE;
	Turret->ModelColor = WHITE;
	Shield->ModelColor = WHITE;
	GunOverCharge = false;
	ShieldOverCharge = false;
	BigShotCount = 0;
	DoubleShotCount = 0;
	MineCount = 0;
	PlasmaShotCount = 0;
#if DEBUG
	BigShotCount = 20; //Turn into keys.
	DoubleShotCount = 40;
	MineCount = 210;
	PlasmaShotCount = 10;
#endif
	MissileCount = 0;
	SecondaryWeapon = None;

	PlaySound(SpawnSound);
}

void ThePlayer::NewGame()
{
	Lives = 4;
	ShieldDrainRate = 25.15f;
	ShieldRechargeRate = 5.15f;
	NextNewLifeScore = 10000;
	GameOver = false;
	Score->Reset();
	Spawn();
}

void ThePlayer::ExtraLife()
{
	NewLife = true;
	Lives++;
}

void ThePlayer::ShieldPowerUp()
{
	ShieldOverCharge = true;
	ShieldPower += 200;
	Shield->ModelColor = BLUE;
	Shield->Alpha = 255.0f;
	if (!PoweredUp) ModelColor = BLUE;
}

void ThePlayer::GunPowerUp()
{
	GunOverCharge = true;
	TurretOverHeat = false;
	TurretHeat -= 150;
	Turret->ModelColor = SKYBLUE;
}

void ThePlayer::FullPowerUp()
{
	Managers.EM.ResetTimer(PowerupTimerID,
		Managers.EM.GetTimerAmount(PowerupTimerID) + PowerUpTimerAmount);
	ModelColor = PURPLE;
	PoweredUp = true;
	PoweredUpRundown = false;
	TurretOverHeat = false;
	Shield->Alpha = 255.0f;
}

void ThePlayer::BigShotPowerUp()
{
	BigShotCount += 22;

	if (SecondaryWeapon == Big) AmmoMeterUpdate(BigShotCount);

	if (DoubleShotCount == 0 && MineCount == 0 &&
		PlasmaShotCount == 0) SwitchSecondaryWeapon(Big);
}

void ThePlayer::DoubleShotPowerUp()
{
	DoubleShotCount += 32;

	if (SecondaryWeapon == Double) AmmoMeterUpdate(DoubleShotCount);

	if (BigShotCount == 0 && MineCount == 0 &&
		PlasmaShotCount == 0) SwitchSecondaryWeapon(Double);
}

void ThePlayer::MinePowerUp()
{
	MineCount += 22;

	if (SecondaryWeapon == Mine) AmmoMeterUpdate(MineCount);

	if (BigShotCount == 0 && DoubleShotCount == 0 &&
		PlasmaShotCount == 0) SwitchSecondaryWeapon(Mine);
}

void ThePlayer::PlasmaShotPowerUp()
{
	PlasmaShotCount += 12;

	if (SecondaryWeapon == Plasma) AmmoMeterUpdate(PlasmaShotCount);

	if (BigShotCount == 0 && MineCount == 0 &&
		DoubleShotCount == 0) SwitchSecondaryWeapon(Plasma);
}

void ThePlayer::PointTurret(float stickDirectionX, float stickDirectionY)
{
	if (stickDirectionX != 0 || stickDirectionY != 0)
	{
		Turret->RotationZ = atan2f(stickDirectionY, stickDirectionX) - RotationZ;
	}
}

void ThePlayer::PointTurret(Vector3 mouseLocation)
{
	Turret->RotationZ = GetAngleFromVectorZ(mouseLocation) - RotationZ;
}

void ThePlayer::FireTurret()
{
	if (TurretOverHeat)	return;

	if (Managers.EM.TimerElapsed(FireRateTimerID))
	{
		for (auto& shot : Shots)
		{
			if (!shot->Enabled)
			{
				PlaySound(FireSound);

				Managers.EM.ResetTimer(FireRateTimerID);

				if (GunOverCharge)
				{
					if (TurretHeat > 0)
					{
						GunOverCharge = false;
						Turret->ModelColor = WHITE;
						TurretHeatMeter->ModelColor = WHITE;
					}
				}
				else
				{
					TurretHeat += 5;
					TurretHeatMeterUpdate();

					if (TurretHeat > TurretHeatMax)
					{
						Managers.EM.ResetTimer(TurretCooldownTimerID);
						TurretOverHeat = true;
					}
				}

				Vector3 turretPosition = Turret->GetWorldPosition();
				Vector3 velocity = GetVelocityFromAngleZ(Turret->WorldRotation.z, 375.0f);
				velocity = Vector3Add(Vector3Multiply(Velocity,
					{ 0.5f, 0.5f, 0.0f }), velocity);
				shot->Spawn(turretPosition, velocity, 2.15f);
				break;
			}
		}
	}

}

void ThePlayer::TurretTimers()
{
	if (TurretOverHeat)
	{
		if (Managers.EM.TimerElapsed(TurretCooldownTimerID))
		{
			TurretOverHeat = false;
			TurretHeat = 0;
			TurretHeatMeterUpdate();
		}
	}
	else
	{
		if (Managers.EM.TimerElapsed(TurretHeatTimerID))
		{
			Managers.EM.ResetTimer(TurretHeatTimerID);

			if (TurretHeat > 2)	TurretHeat -= 2;
			if (TurretHeat >= 1) TurretHeat -= 1;
			TurretHeatMeterUpdate();
		}
	}
}

void ThePlayer::FireSecondary()
{
	if (BigShotCount > 0 && SecondaryWeapon == Big)
	{
		BigShotCount--;
		PlaySound(BigShotSound);
		FireBigShot();
		AmmoMeterUpdate(BigShotCount);

		if (BigShotCount == 0 && MineCount == 0 &&
			PlasmaShotCount == 0) SwitchSecondaryWeapon(Double);

		if (BigShotCount == 0 && DoubleShotCount == 0 &&
			PlasmaShotCount == 0) SwitchSecondaryWeapon(Mine);

		if (BigShotCount == 0 && MineCount == 0 &&
			DoubleShotCount == 0) SwitchSecondaryWeapon(Plasma);

		return;
	}

	if (DoubleShotCount > 0 && SecondaryWeapon == Double)
	{
		DoubleShotCount--;
		PlaySound(DoubleShotSound);
		FireDoubleShot();
		AmmoMeterUpdate(DoubleShotCount);

		if (DoubleShotCount == 0 && MineCount == 0 &&
			PlasmaShotCount == 0) SwitchSecondaryWeapon(Big);

		if (BigShotCount == 0 && DoubleShotCount == 0 &&
			PlasmaShotCount == 0) SwitchSecondaryWeapon(Mine);

		if (BigShotCount == 0 && MineCount == 0 &&
			DoubleShotCount == 0) SwitchSecondaryWeapon(Plasma);

		return;
	}

	if (MineCount > 0 && SecondaryWeapon == Mine)
	{
		MineCount--;
		PlaySound(MineDropSound);
		DropHomingMine();
		AmmoMeterUpdate(MineCount);

		if (BigShotCount == 0 && MineCount == 0 &&
			PlasmaShotCount == 0) SwitchSecondaryWeapon(Double);

		if (DoubleShotCount == 0 && MineCount == 0 &&
			PlasmaShotCount == 0) SwitchSecondaryWeapon(Big);

		if (BigShotCount == 0 && MineCount == 0 &&
			DoubleShotCount == 0) SwitchSecondaryWeapon(Plasma);

		return;
	}

	if (PlasmaShotCount > 0 && SecondaryWeapon == Plasma)
	{
		PlasmaShotCount--;
		PlaySound(PlasmaShotSound);
		FirePlasmaShot();
		AmmoMeterUpdate(PlasmaShotCount);

		if (BigShotCount == 0 && MineCount == 0 &&
			PlasmaShotCount == 0) SwitchSecondaryWeapon(Double);

		if (BigShotCount == 0 && DoubleShotCount == 0 &&
			PlasmaShotCount == 0) SwitchSecondaryWeapon(Mine);

		if (DoubleShotCount == 0 && MineCount == 0 &&
			PlasmaShotCount == 0) SwitchSecondaryWeapon(Big);

		return;
	}
}

void ThePlayer::FireBigShot()
{
	bool spawnNewBigShot = true;
	size_t shotNumber = BigShots.size();

	PlaySound(FireSound);

	for (size_t i = 0; i < shotNumber; i++)
	{
		if (BigShots.at(i)->Enabled == false)
		{
			spawnNewBigShot = false;
			shotNumber = i;
			break;
		}
	}

	if (spawnNewBigShot)
	{
		BigShots.push_back(DBG_NEW Shot());
		Managers.EM.AddLineModel(BigShots.back());
		BigShots.back()->SetModel(BigShotModel);
		BigShots.back()->RotationVelocityZ = 20.0f;
		BigShots.back()->SetScale(3.0f);
		BigShots.back()->BeginRun();
	}

	Vector3 velocity = GetVelocityFromAngleZ(RotationZ, 375.0f);
	velocity = Vector3Add(Vector3Multiply(Velocity,
		{ 0.5f, 0.5f, 0.0f }), velocity);
	BigShots.at(shotNumber)->Spawn(Position, velocity, 2.15f);
	BigShots.at(shotNumber)->HitPoints = 100;
}

void ThePlayer::FireDoubleShot()
{
	float spread = Radius;
	bool spawnNewDoubleShot = true;
	size_t shotNumber = DoubleShots.size();

	PlaySound(FireSound);

	for (size_t i = 0; i < shotNumber; i++)
	{
		if (DoubleShots.at(i)->Enabled == false)
		{
			spawnNewDoubleShot = false;
			shotNumber = i;
			break;
		}
	}

	if (spawnNewDoubleShot)
	{
		DoubleShots.push_back(DBG_NEW Shot());
		Managers.EM.AddLineModel(DoubleShots.back());
		DoubleShots.back()->SetModel(ShotModel);
		DoubleShots.back()->SetScale(3.0f);
		DoubleShots.back()->BeginRun();
	}

	Vector3 offset = GetVelocityFromAngleZ(RotationZ + PI / 2.0f, spread);
	Vector3 position = Position + offset;
	Vector3 velocity = GetVelocityFromAngleZ(RotationZ, 400.0f);
	velocity = Vector3Multiply(Velocity, { 0.5f, 0.5f, 0.0f }) + velocity;
	DoubleShots.at(shotNumber)->Spawn(position, velocity, 2.15f);

	spawnNewDoubleShot = true;
	shotNumber = DoubleShots.size();

	PlaySound(FireSound);

	for (size_t i = 0; i < shotNumber; i++)
	{
		if (DoubleShots.at(i)->Enabled == false)
		{
			spawnNewDoubleShot = false;
			shotNumber = i;
			break;
		}
	}

	if (spawnNewDoubleShot)
	{
		DoubleShots.push_back(DBG_NEW Shot());
		Managers.EM.AddLineModel(DoubleShots.back());
		DoubleShots.back()->SetModel(ShotModel);
		DoubleShots.back()->BeginRun();
		DoubleShots.back()->SetScale(3.0f);
	}

	offset = GetVelocityFromAngleZ(RotationZ + -PI / 2.0f, spread);
	position = Position + offset;
	DoubleShots.at(shotNumber)->Spawn(position, velocity, 2.15f);
}

void ThePlayer::DropHomingMine()
{
	bool spawnNewMine = true;
	size_t mineNumber = Mines.size();

	PlaySound(MineDropSound);

	for (size_t i = 0; i < mineNumber; i++)
	{
		if (Mines.at(i)->Enabled == false)
		{
			spawnNewMine = false;
			mineNumber = i;
			break;
		}
	}

	if (spawnNewMine)
	{
		Mines.push_back(DBG_NEW TheHomingMine());
		Managers.EM.AddLineModel(Mines.back());
		Mines.back()->SetModel(MineModel);
		Mines.back()->ModelColor = GRAY;
		Mines.back()->SetExplodeSound(MineExplodeSound);
		Mines.back()->SetParticleManager(Particles);
		Mines.back()->BeginRun();
	}

	Mines.at(mineNumber)->Spawn(Position);
}

void ThePlayer::FirePlasmaShot()
{
	bool spawnNewBigShot = true;
	size_t shotNumber = PlasmaShots.size();

	PlaySound(FireSound);

	for (size_t i = 0; i < shotNumber; i++)
	{
		if (PlasmaShots.at(i)->Enabled == false)
		{
			spawnNewBigShot = false;
			shotNumber = i;
			break;
		}
	}

	if (spawnNewBigShot)
	{
		PlasmaShots.push_back(DBG_NEW ThePlasmaShot());
		Managers.EM.AddLineModel(PlasmaShots.back());
		PlasmaShots.back()->SetModel(BigShotModel);
		PlasmaShots.back()->RotationVelocityZ = 20.0f;
		PlasmaShots.back()->BeginRun();
	}

	Vector3 velocity = GetVelocityFromAngleZ(RotationZ, 375.0f);
	velocity = Vector3Add(Vector3Multiply(Velocity,
		{ 0.5f, 0.5f, 0.0f }), velocity);
	PlasmaShots.at(shotNumber)->Spawn(Position, velocity);
}

void ThePlayer::CrosshairUpdate()
{
	Crosshair->X(GetMousePosition().x - WindowWidth);
	Crosshair->Y(GetMousePosition().y - WindowHeight);

	if (Crosshair->X() < -WindowWidth)
	{
		Crosshair->X(-WindowWidth);
		SetMousePosition(0, (int)GetMousePosition().y);
	}

	if (Crosshair->X() > WindowWidth)
	{
		Crosshair->X(WindowWidth);
		SetMousePosition((int)WindowWidth * 2, (int)GetMousePosition().y);
	}

	if (Crosshair->Y() < -WindowHeight)
	{
		Crosshair->Y(-WindowHeight);
		SetMousePosition((int)GetMousePosition().x, 0);
	}

	if (Crosshair->Y() > WindowHeight)
	{
		Crosshair->Y((float)WindowHeight);
		SetMousePosition((int)GetMousePosition().x, (int)WindowHeight * 2);
	}
}

void ThePlayer::RotateShip(float amount)
{
	RotationVelocityZ = (amount * 5.5f);
}

void ThePlayer::RotateStop()
{
	RotationVelocityZ = 0.0f;
}

void ThePlayer::ThrustOn(float amount)
{
	if (!Enabled || GetBeenHit()) return;

	SetAccelerationToMaxAtRotation((amount * 50.25f), 350.0f);
	Flame->Enabled = true;

	if (Managers.EM.TimerElapsed(FlameColorTimerID))
	{
		Managers.EM.ResetTimer(FlameColorTimerID, GetRandomFloat(0.025f, 0.25f));

		Color color = {(unsigned char)GetRandomValue(100, 255),
			(unsigned char)GetRandomValue(60, 105),
			(unsigned char)GetRandomValue(0, 255), 255};

		Flame->ModelColor = color;
	}

	if (!IsSoundPlaying(ThrustSound)) PlaySound(ThrustSound);
}

void ThePlayer::ThrustOff()
{
	StopSound(ThrustSound);
	SetAccelerationToZero(0.45f);
	Flame->Enabled = false;
}

void ThePlayer::ShieldOn()
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

void ThePlayer::ShieldOff()
{
	if (PoweredUp) return;

	StopSound(ShieldOnSound);
	Shield->Enabled = false;
}

void ThePlayer::ShieldPowerDrain(float deltaTime)
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

void ThePlayer::WeHaveThePower()
{
	if (ShieldPower < 100.0f) ShieldPower = 100.0f;

	if (TurretHeat > 0) TurretHeat = 0;

	ShieldOn();

	if (Managers.EM.TimerElapsed(PowerupTimerID) && !PoweredUpRundown)
	{
		PoweredUpRundown = true;
		Managers.EM.ResetTimer(PowerupRundownTimerID);
		Managers.EM.ResetTimer(PowerUpBlinkTimerID);
	}

	if (PoweredUpRundown)
	{
		if (Managers.EM.TimerElapsed(PowerUpBlinkTimerID))
		{
			Managers.EM.ResetTimer(PowerUpBlinkTimerID);

			if (ModelColor.g == 255.0f)
			{
				ModelColor = PURPLE;
			}
			else
			{
				ModelColor = WHITE;
			}
		}
	}

	if (Managers.EM.TimerElapsed(PowerupRundownTimerID) && PoweredUpRundown)
	{
		PoweredUp = false;
		PoweredUpRundown = false;

		if (ShieldOverCharge) ModelColor = BLUE;
		else ModelColor = WHITE;

		Managers.EM.SetTimer(PowerupTimerID, 0.0f);
	}
}

void ThePlayer::ShieldHit(Vector3 location, Vector3 velocity)
{
	PlaySound(ShieldHitSound);
	Acceleration = {0};

	Velocity = Vector3Add(Vector3Multiply(Vector3Multiply(Velocity, {0.25f}), {-1}),
		Vector3Add(Vector3Multiply(velocity, {0.90f}),
			GetVelocityFromAngleZ(GetAngleFromVectorsZ(location, Position),	196.666f)));

	if (PoweredUp) return;

	if (ShieldPower > 20)
	{
		ShieldPower -= 20;
	}
	else
	{
		ShieldPower = 0;
	}
}

void ThePlayer::TurretHeatMeterUpdate()
{
	if (!GunOverCharge)
	{
		TurretHeatMeter->Scale = TurretHeat * 0.01f;

		if (TurretHeat > 50.0f)
		{
			TurretHeatMeter->ModelColor = Color{ 200, 200, 200, 255 };

			if (TurretHeat > 75.0f)
			{
				TurretHeatMeter->ModelColor = Color{ 173, 216, 255, 255 };

				if (TurretHeat > 90.0f)
				{
					TurretHeatMeter->ModelColor = YELLOW;

					if (TurretHeat > 99.0f)
					{
						TurretHeatMeter->ModelColor = RED;
					}
				}
			}
		}
		else
		{
			TurretHeatMeter->ModelColor = Color{ 155, 155, 155, 200 };
		}
	}
	else
	{
		TurretHeatMeter->ModelColor = SKYBLUE;
		TurretHeatMeter->Scale = 1.0f;
	}
}

void ThePlayer::AmmoMeterUpdate(int ammoCount)
{
	if (ammoCount <= 0)	SwitchSecondaryWeapon(None);

	float ammoX = 3.25f;
	float scale = 1.0f;
	Color color = LIGHTGRAY;

	if (ammoCount > 100)
	{
		ammoCount /= 100;
		ammoX = 8.5f;
		scale = 3.0f;
		color = DARKGRAY;
	} else if (ammoCount > 10)
	{
		ammoCount /= 10;
		ammoX = 5.5f;
		scale = 2.0f;
		color = GRAY;
	}

	if (AmmoMeterModels.size() < ammoCount)
		AddAmmoMeterModels(ammoCount - (int)AmmoMeterModels.size());

	Vector2 position = { 0.0f, 30.0f };

	for (const auto& model : AmmoMeterModels)
	{
		model->Enabled = false;
		model->Position = { position.x, position.y, 0.0f };
		position.x += 1.0f;
	}

	for (int i = 0; i < ammoCount; i++)
	{
		AmmoMeterModels[i]->Enabled = true;
		AmmoMeterModels[i]->Scale = scale;
		AmmoMeterModels[i]->ModelColor = color;
		AmmoMeterModels[i]->X(i * ammoX);
	}
}

void ThePlayer::AddAmmoMeterModels(int count)
{
	for (int i = 0; i < count; i++)
	{
		AmmoMeterModels.push_back(DBG_NEW LineModel());
		Managers.EM.AddLineModel(AmmoMeterModels.back());
		AmmoMeterModels.back()->SetModel(ShotModel);
		AmmoMeterModels.back()->SetParent(*this);
		AmmoMeterModels.back()->RotationLocked = true;
		AmmoMeterModels.back()->Scale = 0.5f;
		AmmoMeterModels.back()->Radius = 0.0f;
		AmmoMeterModels.back()->BeginRun();
	}
}

void ThePlayer::WeaponPlasmaIconUpdate(float deltaTime)
{
	if (!WeaponTypeIconPlasma->Enabled) return;

	WeaponTypeIconPlasma->Scale += deltaTime * 0.25f;

	if (WeaponTypeIconPlasma->Scale > 0.5f)
	{
		WeaponTypeIconPlasma->Scale = 0.25f;
	}
}

void ThePlayer::SwitchSecondaryWeapon(SecondaryWeaponType type)
{
	WeaponTypeIconBig->Enabled = false;
	WeaponTypeIconDoubleLeft->Enabled = false;
	WeaponTypeIconDoubleRight->Enabled = false;
	WeaponTypeIconPlasma->Enabled = false;
	WeaponTypeIconMine->Enabled = false;

	if (type == Big && BigShotCount == 0) type = Double;
	if (type == Double && DoubleShotCount == 0)	type = Plasma;
	if (type == Plasma && PlasmaShotCount == 0)	type = Mine;
	if (type == Mine && MineCount == 0)	type = Big;
	if (type == Big && BigShotCount == 0) type = Double;
	if (type == Double && DoubleShotCount == 0)	type = Plasma;

	if (BigShotCount == 0 && DoubleShotCount == 0 &&
		PlasmaShotCount == 0 && MineCount == 0)
	{
		type = None;
	}

	if (type == Big) WeaponTypeIconBig->Enabled = true;
	if (type == Big) AmmoMeterUpdate(BigShotCount);
	if (type == Double) WeaponTypeIconDoubleLeft->Enabled = true;
	if (type == Double) WeaponTypeIconDoubleRight->Enabled = true;
	if (type == Double) AmmoMeterUpdate(DoubleShotCount);
	if (type == Plasma) WeaponTypeIconPlasma->Enabled = true;
	if (type == Plasma)	WeaponTypeIconPlasma->Scale = 0.25f;
	if (type == Plasma) AmmoMeterUpdate(PlasmaShotCount);
	if (type == Mine) WeaponTypeIconMine->Enabled = true;
	if (type == Mine) AmmoMeterUpdate(MineCount);

	SecondaryWeapon = type;
}

void ThePlayer::IsSecondaryWeaponSwitched(float next)
{
}

void ThePlayer::Gamepad()
{
	//Axis 0 is -1 for Left, 1 for right on left stick.
	//Axis 1 is -1 for Up, 1 for Down on left stick.
	//Axis 2 is -1 for Left, 1 for right on right stick.
	//Axis 3 is -1 for Up, 1 for Down on right stick.

	//Button A
	if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) ||
		IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_THUMB))
	{
		FireSecondary();
	}

	//Left Stick Left/Right
	if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) > 0.1f) //Right
	{
		RotateShip(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X));
	}
	else if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) < -0.1f) //Left
	{
		RotateShip(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X));
	}
	else
	{
		RotateStop();
	}

	//Left Stick Up/Down
	if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) > 0.1f) //Down
	{
	}
	else if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) < -0.1f) //Up
	{
	}
	else
	{
	}

	PointTurret(GetGamepadAxisMovement(0, 2), GetGamepadAxisMovement(0, 3));

	//Right Stick
	if (GetGamepadAxisMovement(0, 2) > 0.1f ||
		GetGamepadAxisMovement(0, 2) > 0.1f) //Left/Right
	{
		FireTurret();
	}
	//else if (GetGamepadAxisMovement(0, 2) < -0.1f) //Left
	//{
	//	FireTurret();
	//}
	else if (GetGamepadAxisMovement(0, 3) > 0.1f ||
		GetGamepadAxisMovement(0, 3) > 0.1f) //Up/Down
	{
		FireTurret();
	}
	//else if (GetGamepadAxisMovement(0, 3) < -0.1f) //Up
	//{
	//	FireTurret();
	//}

	//Left Trigger
	if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
	{
		ShieldOn();
	}
	else
	{
		ShieldOff();
	}

	//Right Bumper
	if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
	{
		MouseWheelScroll = SecondaryWeapon;

		MouseWheelScroll += 1;

		if (MouseWheelScroll < 1) MouseWheelScroll = 4;
		if (MouseWheelScroll > 4) MouseWheelScroll = 1;

		SwitchSecondaryWeapon((SecondaryWeaponType)MouseWheelScroll);
	}

	//Right Trigger
	if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_TRIGGER) > 0.1f)
	{
		ThrustOn(GetGamepadAxisMovement(0, 4));
	}
	else
	{
		ThrustOff();
	}

	//Left Bumper
	if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1))
	{
		MouseWheelScroll = SecondaryWeapon;

		MouseWheelScroll -= 1;

		if (MouseWheelScroll < 1) MouseWheelScroll = 4;
		if (MouseWheelScroll > 4) MouseWheelScroll = 1;

		SwitchSecondaryWeapon((SecondaryWeaponType)MouseWheelScroll);
	}
}

void ThePlayer::Keyboard()
{
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
	{
		RotateShip(1.0f);
	}
	else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
	{
		RotateShip(-1.0f);
	}
	else
	{
		RotateStop();
	}

	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
	{
		ThrustOn(1.0f);
	}
	else
	{
		ThrustOff();
	}

	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_SPACE)
		|| IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		ShieldOn();
	}
	else
	{
		ShieldOff();
	}

	if (IsKeyPressed(KEY_E))
	{
		FireSecondary();
	}

	if (IsKeyPressed(KEY_F))
	{
		MouseWheelScroll = SecondaryWeapon;

		MouseWheelScroll += 1;
		//Repeated code.
		if (MouseWheelScroll < 1) MouseWheelScroll = 4;
		if (MouseWheelScroll > 4) MouseWheelScroll = 1;

		SwitchSecondaryWeapon((SecondaryWeaponType)MouseWheelScroll);
	}
}

void ThePlayer::Mouse()
{
	Crosshair->Enabled = true;

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		PointTurret(Crosshair->Position);
		FireTurret();
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
	{
		FireSecondary();
	}

	MouseWheel();
}

void ThePlayer::MouseWheel()
{
	if ((int)GetMouseWheelMove() != 0)
	{
		MouseWheelScroll = SecondaryWeapon;

		MouseWheelScroll -= (int)GetMouseWheelMove();
		//Repeated code.
		if (MouseWheelScroll < 1) MouseWheelScroll = 4;
		if (MouseWheelScroll > 4) MouseWheelScroll = 1;

		SwitchSecondaryWeapon((SecondaryWeaponType)MouseWheelScroll);
	}
}

void ThePlayer::AltMouse()
{
	Crosshair->Enabled = false;

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		FireSecondary();
	}

	float sensitivity = 0.0025f;

	float mouseX = GetMouseDelta().x * sensitivity;
	float mouseY = GetMouseDelta().y * sensitivity;

	mouseX = Clamp(mouseX, -1.0f, 1.0f);
	mouseY = Clamp(mouseY, -1.0f, 1.0f);

	float over = 0.015f;

	if (mouseX > over || mouseX < -over) //Right
	{
		PointTurret(mouseX, mouseY);
		FireTurret();
	}
	else if (mouseY > over || mouseY < -over) //Down
	{
		PointTurret(mouseX, mouseY);
		FireTurret();
	}

	MouseWheel();
}
