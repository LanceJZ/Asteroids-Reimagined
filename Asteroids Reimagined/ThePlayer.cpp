#include "ThePlayer.h"

ThePlayer::ThePlayer()
{
	EM.AddLineModel(Crosshair = DBG_NEW LineModel());
	EM.AddLineModel(TurretHeatMeter = DBG_NEW LineModel());
	EM.AddLineModel(WeaponTypeIconDoubleLeft = DBG_NEW LineModel());
	EM.AddLineModel(WeaponTypeIconDoubleRight = DBG_NEW LineModel());
	EM.AddLineModel(WeaponTypeIconBig = DBG_NEW LineModel());
	EM.AddLineModel(WeaponTypeIconMine = DBG_NEW LineModel());
	EM.AddLineModel(WeaponTypeIconPlasma = DBG_NEW LineModel());

	EM.AddOnScreenText(Score = DBG_NEW TheScoreOld());

	PowerupRundownTimerID = EM.AddTimer(2.0f);
	PowerUpBlinkTimerID = EM.AddTimer(0.25f);

	MagazineSize = 16;
}

ThePlayer::~ThePlayer()
{
}

void ThePlayer::SetShotModel(std::vector<Vector3> model)
{
	ThePlayerControls::SetShotModel(model);		

	WeaponTypeIconDoubleLeft->SetModel(model);
	WeaponTypeIconDoubleRight->SetModel(model);
}

void ThePlayer::SetTurretHeatModel(std::vector<Vector3> model)
{
	TurretHeatMeter->SetModel(model);
}

void ThePlayer::SetBigShotModel(std::vector<Vector3> model)
{
	ThePlayerControls::SetBigShotModel(model);
	WeaponTypeIconBig->SetModel(model);
	WeaponTypeIconPlasma->SetModel(model);
}

void ThePlayer::SetMineModel(std::vector<Vector3> model)
{
	ThePlayerControls::SetMineModel(model);
	WeaponTypeIconMine->SetModel(model);
}

void ThePlayer::SetFireSound(Sound sound)
{
	ThePlayerControls::SetFireSound(sound);
	SetSoundVolume(FireSound, 0.2f);
}

void ThePlayer::SetExplodeSound(Sound sound)
{
	ThePlayerControls::SetExplodeSound(sound);
	SetSoundVolume(ExplodeSound, 0.95f);
}

void ThePlayer::SetThrustSound(Sound sound)
{
	ThePlayerControls::SetThrustSound(sound);
	SetSoundVolume(ThrustSound, 0.5f);
}

void ThePlayer::SetBonusSound(Sound sound)
{
	BonusSound = sound;
}

void ThePlayer::SetBigShotSound(Sound sound)
{
	ThePlayerControls::SetBigShotSound(sound);
	SetSoundVolume(BigShotSound, 0.25f);
}

void ThePlayer::SetDoubleShotSound(Sound sound)
{
	ThePlayerControls::SetDoubleShotSound(sound);
	SetSoundVolume(DoubleShotSound, 0.5f);
}

void ThePlayer::SetGunHotSound(Sound sound)
{
	GunHotSound = sound;
}

void ThePlayer::SetShieldLowSound(Sound sound)
{
	ShieldLowSound = sound;
	SetSoundVolume(ShieldLowSound, 0.33f);
}

void ThePlayer::SetPowerUpWarningSound(Sound sound)
{
	PowerUpWarningSound = sound;
	SetSoundVolume(PowerUpWarningSound, 0.25f);
}

void ThePlayer::SetCrosshairModel(std::vector<Vector3> model)
{
	Crosshair->SetModel(model);
	Crosshair->NoCollision = true;
}

bool ThePlayer::Initialize()
{
	ThePlayerControls::Initialize();

	Crosshair->Enabled = false;
	WeaponTypeIconBig->Enabled = false;
	WeaponTypeIconDoubleLeft->Enabled = false;
	WeaponTypeIconDoubleRight->Enabled = false;
	WeaponTypeIconPlasma->Enabled = false;
	WeaponTypeIconMine->Enabled = false;

	PowerUpTimerAmount = 8.0f;

	Flame->ModelColor = { 255, 140, 0, 255 };

	return true;
}

bool ThePlayer::BeginRun()
{
	ThePlayerControls::BeginRun();

	Turret->X(-9.0f);

	TurretHeatMeter->Y(-32.0f);
	TurretHeatMeter->SetParent(*this);
	TurretHeatMeter->Enabled = false;
	TurretHeatMeter->Radius = 0.0f;
	TurretHeatMeter->IgnoreParentRotation = true;

	WeaponTypeIconBig->SetParent(*this);
	WeaponTypeIconDoubleLeft->SetParent(*this);
	WeaponTypeIconDoubleRight->SetParent(*this);
	WeaponTypeIconPlasma->SetParent(*this);
	WeaponTypeIconMine->SetParent(*this);

	WeaponTypeIconBig->IgnoreParentRotation = true;
	WeaponTypeIconDoubleLeft->IgnoreParentRotation = true;
	WeaponTypeIconDoubleRight->IgnoreParentRotation = true;
	WeaponTypeIconPlasma->IgnoreParentRotation = true;
	WeaponTypeIconMine->IgnoreParentRotation = true;

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

	return true;
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
	ThePlayerControls::Update(deltaTime);

	WeaponPlasmaIconUpdate(deltaTime);
	CrosshairUpdate();

	if (PoweredUp) WeHaveThePower();
}

void ThePlayer::FixedUpdate(float deltaTime)
{
	ThePlayerControls::FixedUpdate(deltaTime);

	TurretTimers();
}

void ThePlayer::Draw3D()
{
	LineModel::Draw3D();

}

void ThePlayer::Hit()
{
	ThePlayerControls::Hit();

	Lives--;
	Crosshair->Enabled = false;
	TurretHeatMeter->Enabled = false;
	WeaponTypeIconBig->Enabled = false;
	WeaponTypeIconDoubleLeft->Enabled = false;
	WeaponTypeIconDoubleRight->Enabled = false;
	WeaponTypeIconPlasma->Enabled = false;
	WeaponTypeIconMine->Enabled = false;

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

void ThePlayer::ScoreUpdate(int addToScore)
{
	if (GameOver) return;

	//TODO: Convert this to use engine score class.
	Score->UpdateScore(addToScore);

	if (Score->GetScore() > NextNewLifeScore)
	{
		NextNewLifeScore += ((10000 * (WaveNumber + 1)) * 4);
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

	Turret->Enabled = true;
	TurretHeatMeter->Enabled = true;
	ShieldPower = 100.0f;
	TurretHeatMeter->Scale = 0.0f;
	TurretOverheat = false;
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
	//BigShotCount = 20; //Turn into keys.
	//DoubleShotCount = 40;
	//MineCount = 210;
	//PlasmaShotCount = 10;
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
	ThePlayerControls::ShieldPowerUp();

	Shield->ModelColor = BLUE;
	if (!PoweredUp) ModelColor = BLUE;
}

void ThePlayer::GunPowerUp()
{
	GunOverCharge = true;
	TurretOverheat = false;
	TurretHeat -= 150;
	Turret->ModelColor = SKYBLUE;
}

void ThePlayer::FullPowerUp()
{
	ThePlayerControls::FullPowerUp();

	ModelColor = PURPLE;
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
	ThePlayerControls::PointTurret(mouseLocation);

}

void ThePlayer::FireTurret()
{
	ThePlayerControls::FireTurret();

	if (EM.TimerElapsed(FireRateTimerID))
	{
		for (auto& shot : Shots)
		{
			if (!shot->Enabled)
			{
				PlaySound(FireSound);

				EM.ResetTimer(FireRateTimerID);

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
					TurretHeatMeterUpdate();

					if (TurretHeat > TurretHeatMax * 0.8f) 
						PlaySound(GunHotSound);

					if (TurretHeat > TurretHeatMax)
					{
						EM.ResetTimer(TurretCooldownTimerID);
						TurretOverheat = true;
					}
					else TurretHeat += 5;
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
	ThePlayerControls::TurretTimers();

	if (TurretOverheat)
	{
		if (TurretOverheatCooldown)
		{
			TurretHeatMeterUpdate();
			TurretOverheatCooldown = false;
		}
	}
	else
	{
		if (TurretOverheatRundown)
		{
			TurretOverheatRundown = false;
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
		EM.AddLineModel(BigShots.back());
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
		EM.AddLineModel(DoubleShots.back());
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
		EM.AddLineModel(DoubleShots.back());
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
		EM.AddLineModel(Mines.back());
		Mines.back()->SetModel(MineModel);
		Mines.back()->ModelColor = GRAY;
		Mines.back()->SetExplodeSound(MineExplodeSound);
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
		EM.AddLineModel(PlasmaShots.back());
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
	Crosshair->X(GetMousePosition().x - WindowHalfWidth);
	Crosshair->Y(GetMousePosition().y - WindowHalfHeight);

	if (Crosshair->X() < -WindowHalfWidth)
	{
		Crosshair->X((float)-WindowHalfWidth);
		SetMousePosition(0, (int)GetMousePosition().y);
	}

	if (Crosshair->X() > WindowHalfWidth)
	{
		Crosshair->X((float)WindowHalfWidth);
		SetMousePosition((int)WindowHalfWidth * 2, (int)GetMousePosition().y);
	}

	if (Crosshair->Y() < -WindowHalfHeight)
	{
		Crosshair->Y((float)-WindowHalfHeight);
		SetMousePosition((int)GetMousePosition().x, 0);
	}

	if (Crosshair->Y() > WindowHalfHeight)
	{
		Crosshair->Y((float)WindowHalfHeight);
		SetMousePosition((int)GetMousePosition().x, (int)WindowHalfHeight * 2);
	}
}

void ThePlayer::RotateShip(float amount)
{
	ThePlayerControls::RotateShip(amount);
}

void ThePlayer::RotateStop()
{
	ThePlayerControls::RotateStop();
}

void ThePlayer::ThrustOn(float amount)
{
	ThePlayerControls::ThrustOn(amount);
}

void ThePlayer::ThrustOff()
{
	ThePlayerControls::ThrustOff();
}

void ThePlayer::ShieldOn()
{
	ThePlayerControls::ShieldOn();

	if (ShieldPower > 0.0f)
	{
		if (!ShieldOverCharge && !IsSoundPlaying(ShieldLowSound)) if (ShieldPower < 30.0f) PlaySound(ShieldLowSound);
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
	ThePlayerControls::ShieldPowerDrain(deltaTime);
}

void ThePlayer::WeHaveThePower()
{
	ThePlayerControls::WeHaveThePower();

	if (EM.TimerElapsed(PowerupTimerID) && !PoweredUpRundown)
	{
		PoweredUpRundown = true;
		EM.ResetTimer(PowerupRundownTimerID);
		EM.ResetTimer(PowerUpBlinkTimerID);
	}

	if (PoweredUpRundown)
	{
		if (EM.TimerElapsed(PowerUpBlinkTimerID))
		{
			EM.ResetTimer(PowerUpBlinkTimerID);

			PlaySound(PowerUpWarningSound);

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

	if (EM.TimerElapsed(PowerupRundownTimerID) && PoweredUpRundown)
	{
		PoweredUp = false;
		PoweredUpRundown = false;

		if (ShieldOverCharge) ModelColor = BLUE;
		else ModelColor = WHITE;

		EM.SetTimer(PowerupTimerID, 0.0f);
	}
}

void ThePlayer::ShieldHit(Vector3 position, Vector3 velocity)
{
	//TODO:: Have rocks and enemy check for shield radius if on.

	PlaySound(ShieldHitSound);
	Acceleration = {0};
	Velocity = GetReflectionVelocity(position, velocity, 106.666f, 0.90f, 0.25f);

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

void ThePlayer::SetAntiPlayer(Entity* entity)
{
	AntiPlayer = entity;
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

	Vector2 position = { 0.0f, 32.0f };

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
		EM.AddLineModel(AmmoMeterModels.back());
		AmmoMeterModels.back()->SetModel(ShotModel);
		AmmoMeterModels.back()->SetParent(*this);
		AmmoMeterModels.back()->IgnoreParentRotation = true;
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
		GetGamepadAxisMovement(0, 2) < -0.1f) //Left/Right
	{
		FireTurret();
	}
	else if (GetGamepadAxisMovement(0, 3) > 0.1f ||
		GetGamepadAxisMovement(0, 3) < -0.1f) //Up/Down
	{
		FireTurret();
	}

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
	if (IsKeyDown(KEY_D))
	{
		RotateShip(1.0f);
	}
	else if (IsKeyDown(KEY_A))
	{
		RotateShip(-1.0f);
	}
	else
	{
		RotateStop();
	}

	if (IsKeyPressed(KEY_M))
	{
		Crosshair->Enabled = !Crosshair->Enabled;
		MouseTurnedOff = !MouseTurnedOff;
	}

	if (IsKeyDown(KEY_RIGHT))
	{
		Turret->RotationAccelerationZ += 20.25f;
	}
	else if (IsKeyDown(KEY_LEFT))
	{
		Turret->RotationAccelerationZ -= 20.25f;
	}
	else
	{
		Turret->RotationAccelerationZ = 0.0f;
		Turret->RotationVelocityZ = 0.0f;
	}

	if (IsKeyDown(KEY_DOWN))
	{
		FireSecondary();
	}

	if (IsKeyDown(KEY_UP))
	{
		FireTurret();
	}

	if (IsKeyDown(KEY_W))
	{
		ThrustOn(1.0f);
	}
	else
	{
		ThrustOff();
	}

	if (IsKeyDown(KEY_SPACE)
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
	if (!MouseTurnedOff) Crosshair->Enabled = true;

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
