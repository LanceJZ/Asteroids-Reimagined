#include "ThePlayer.h"

ThePlayer::ThePlayer()
{
	Managers.EM.AddLineModel(Flame = DBG_NEW LineModel());
	Managers.EM.AddLineModel(Shield = DBG_NEW LineModel());
	Managers.EM.AddLineModel(Turret = DBG_NEW LineModel());
	Managers.EM.AddLineModel(Crosshair = DBG_NEW LineModel());
	Managers.EM.AddLineModel(TurretHeatMeter = DBG_NEW LineModel());
	Managers.EM.AddOnScreenText(Score = DBG_NEW TheScore());

	FireRateTimerID = Managers.EM.AddTimer(0.125f);
	TurretCooldownTimerID = Managers.EM.AddTimer(2.0f);
	TurretHeatTimerID = Managers.EM.AddTimer(0.15f);
	PowerupTimerID = Managers.EM.AddTimer(8.0f);
	PowerupRundownTimerID = Managers.EM.AddTimer(2.0f);
	PowerUpBlinkTimerID = Managers.EM.AddTimer(0.25f);

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
	}
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

	return false;
}

void ThePlayer::Input()
{
	LineModel::Input();

	if (GameOver) return;


	if (IsGamepadAvailable(0) && Enabled)
	{
		Gamepad();
		Crosshair->Enabled = false;
	}
	else if (Enabled)
	{
		Crosshair->Enabled = true;
		Keyboard();
	}
	else
	{
		Crosshair->Enabled = false;
	}
}

void ThePlayer::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	CheckScreenEdge();
	TurretTimers();
	CrosshairUpdate();
	ShieldPowerDrain(deltaTime);

	if (PoweredUp)
	{
		WeHaveThePower();
		ShieldOn();
	}
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
		Enabled = false;
		Turret->Enabled = false;
		Flame->Enabled = false;
		Crosshair->Enabled = false;
		TurretHeatMeter->Enabled = false;

		if (Lives < 0)
		{
			GameOver = true;
		}
	}

}

void ThePlayer::ScoreUpdate(int addToScore)
{
	Score->UpdateScore(addToScore);

	if (Score->GetScore() > NextNewLifeScore)
	{
		NextNewLifeScore += 10000;
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
	Position = { 0, 0, 0 };
	Velocity = { 0, 0, 0 };
	Enabled = true;
	Flame->Enabled = false;
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
	ShieldPower = 300;
	Shield->ModelColor = BLUE;
	Shield->Alpha = 255.0f;
}

void ThePlayer::GunPowerUp()
{
	GunOverCharge = true;
	TurretOverHeat = false;
	TurretHeat = -200;
	TurretHeatMeter->Scale = 0.0f;
	Turret->ModelColor = RED;
	TurretHeatMeter->ModelColor = BLUE;
}

void ThePlayer::FullPowerUp()
{
	Managers.EM.ResetTimer(PowerupTimerID);
	ModelColor = PURPLE;
	PoweredUp = true;
	PoweredUpRundown = false;
	TurretOverHeat = false;
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
	Turret->RotationZ = AngleFromVectorZ(mouseLocation) - RotationZ;
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
				TurretHeat += 5;
				TurretHeatMeterUpdate();

				if (TurretHeat > TurretHeatMax)
				{
					Managers.EM.ResetTimer(TurretCooldownTimerID);
					TurretOverHeat = true;
				}

				if (GunOverCharge)
				{
					if (TurretHeat > 0)
					{
						GunOverCharge = false;
						Turret->ModelColor = WHITE;
						TurretHeatMeter->ModelColor = WHITE;
					}
				}

				Vector3 turretPosition = Turret->GetWorldPosition();
				Vector3 velocity = GetVelocityFromAngleZ(Turret->WorldRotation.z, 375.0f);
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

void ThePlayer::RotateLeft(float amount)
{
	RotationVelocityZ = (amount * 5.5f);
}

void ThePlayer::RotateRight(float amount)
{
	RotationVelocityZ = (amount * 5.5f);
}

void ThePlayer::RotateStop()
{
	RotationVelocityZ = 0.0f;
}

void ThePlayer::ThrustOn(float amount)
{
	SetAccelerationToMaxAtRotation((amount * 50.25f), 350.0f);
	Flame->Enabled = true;

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
	StopSound(ShieldOnSound);
	Shield->Enabled = false;
}

void ThePlayer::ShieldPowerDrain(float deltaTime)
{
	if (Shield->Enabled)
	{
		ShieldPower -= ShieldDrainRate * deltaTime;

		if (ShieldPower < 0.0f)
		{
			ShieldPower = 0.0f;
		}
	}
	else
	{
		ShieldPower += ShieldRechargeRate * deltaTime;
	}
}

void ThePlayer::WeHaveThePower()
{
		ShieldPower = 100.0f;
		TurretHeat = 0;

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
			ModelColor = WHITE;
		}
}

void ThePlayer::ShieldHit(Vector3 location, Vector3 velocity)
{
	PlaySound(ShieldHitSound);
	Acceleration = {0};

	Velocity = Vector3Add(Vector3Multiply(Vector3Multiply(Velocity, {0.25f}), {-1}),
		Vector3Add(Vector3Multiply(velocity, {0.90f}),
			GetVelocityFromAngleZ(GetAngleFromVectorsZ(location, Position),	196.666f)));

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
		TurretHeatMeter->Scale = (TurretHeat * 0.01f) * -1.0f;
	}
}

void ThePlayer::Gamepad()
{
	//Button B is 6 for Shield //Button A is 7 for Fire //Button Y is 8 for Hyperspace
	//Button X is 5	//Left bumper is 9 //Right bumper is 11 //Left Trigger is 10
	//Right Trigger is 12 for Shield //Dpad Up is 1 for	//Dpad Down is 3 for
	//Dpad Left is 4 for rotate left //Dpad Right is 2 for rotate right
	//Axis 1 is -1 for Up, 1 for Down on left stick.
	//Axis 0 is -1 for Left, 1 for right on left stick.
	//Axis 3 is -1 for Up, 1 for Down on right stick.
	//Axis 2 is -1 for Left, 1 for right on right stick.

	//Left Stick
	if (GetGamepadAxisMovement(0, 0) > 0.1f) //Right
	{
		RotateRight(GetGamepadAxisMovement(0, 0));
	}
	else if (GetGamepadAxisMovement(0, 0) < -0.1f) //Left
	{
		RotateLeft(GetGamepadAxisMovement(0, 0));
	}
	else
	{
		RotateStop();
	}

	if (GetGamepadAxisMovement(0, 1) > 0.1f) //Down
	{
	}
	else if (GetGamepadAxisMovement(0, 1) < -0.1f) //Up
	{
		ThrustOn(-GetGamepadAxisMovement(0, 1));
	}
	else
	{
		ThrustOff();
	}

	//Right Stick
	PointTurret(GetGamepadAxisMovement(0, 2), GetGamepadAxisMovement(0, 3));

	if (GetGamepadAxisMovement(0, 2) > 0.1f) //Right
	{
		FireTurret();
	}
	else if (GetGamepadAxisMovement(0, 2) < -0.1f) //Left
	{
		FireTurret();
	}

	if (GetGamepadAxisMovement(0, 3) > 0.1f) //Down
	{
		FireTurret();
	}
	else if (GetGamepadAxisMovement(0, 3) < -0.1f) //Up
	{
		FireTurret();
	}

	//Right Trigger
	if (IsGamepadButtonDown(0, 12))
	{
		ShieldOn();
	}
	else
	{
		ShieldOff();
	}

	if (IsGamepadButtonDown(0, 4) || GetGamepadAxisMovement(0, 0) < -0.25f)
	{
	}
	else if (IsGamepadButtonDown(0, 2) || GetGamepadAxisMovement(0, 0) > 0.25f)
	{
	}

	if (IsGamepadButtonPressed(0, 7))
	{
	}

	if (IsGamepadButtonDown(0, 11) || IsGamepadButtonDown(0, 6))
	{
	}
	else
	{
	}

	if (IsGamepadButtonDown(0, 5) || IsGamepadButtonDown(0, 8)) //X button
	{
	}
	else
	{
	}
}

void ThePlayer::Keyboard()
{
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
	{
		RotateRight(1.0f);
	}
	else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
	{
		RotateLeft(-1.0f);
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

	if (IsKeyPressed(KEY_RIGHT_CONTROL) || IsKeyPressed(KEY_LEFT_CONTROL) ||
		IsKeyPressed(KEY_SPACE))
	{
	}

	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_E) || IsKeyDown(KEY_S)
		|| IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_SPACE))
	{
		ShieldOn();
	}
	else
	{
		ShieldOff();
	}

	PointTurret(Crosshair->Position);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		FireTurret();
	}
}