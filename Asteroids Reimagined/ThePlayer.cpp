#include "ThePlayer.h"

ThePlayer::ThePlayer()
{
	TheManagers.EM.AddLineModel(Flame = DBG_NEW LineModel());
	TheManagers.EM.AddLineModel(Shield = DBG_NEW LineModel());
	TheManagers.EM.AddLineModel(Turret = DBG_NEW LineModel());
	TheManagers.EM.AddLineModel(Crosshair = DBG_NEW LineModel());
	TheManagers.EM.AddTimer(ShotTimerID = TheManagers.EM.AddTimer());
	TheManagers.EM.AddTimer(TurretCooldownTimerID = TheManagers.EM.AddTimer());
	TheManagers.EM.AddTimer(TurretHeatTimerID = TheManagers.EM.AddTimer());

	for (int i = 0; i < 8; i++)
	{
		Shots[i] = DBG_NEW Shot();
		TheManagers.EM.AddLineModel(Shots[i]);
	}
}

ThePlayer::~ThePlayer()
{
}

bool ThePlayer::Initialize(Utilities* utilities)
{
	LineModel::Initialize(TheUtilities);
	//Scale = 35.0f;

	Flame->Enabled = false;
	Shield->Enabled = false;

	return false;
}

bool ThePlayer::BeginRun()
{
	Flame->SetParent(this);
	Shield->SetParent(this);

	Turret->X(-9.0f);
	Turret->SetParent(this);

	return false;
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
	Flame->Radius = 0.0f;
}

void ThePlayer::SetShieldModel(LineModelPoints model)
{
	Shield->SetModel(model);
}

void ThePlayer::SetCrosshairModel(LineModelPoints model)
{
	Crosshair->SetModel(model);
	Crosshair->Radius = 0.0f;
}

void ThePlayer::Input()
{
	LineModel::Input();


	if (IsGamepadAvailable(0))
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
}

void ThePlayer::Draw3D()
{
	LineModel::Draw3D();

}

void ThePlayer::Hit()
{
	Acceleration = { 0 };
	Velocity = { 0 };
	Lives--;
	Enabled = false;
	Turret->Enabled = false;
}

void ThePlayer::ScoreUpdate(int addToScore)
{
	Score += addToScore;

	if (Score > HighScore)
	{
		HighScore = Score;
	}

	if (Score > NextNewLifeScore)
	{
		NextNewLifeScore += 10000;
		Lives++;
		NewLife = true;
	}
}

void ThePlayer::Reset()
{
	Position = { 0, 0, 0 };
	Velocity = { 0, 0, 0 };
	Enabled = true;
	Flame->Enabled = false;
	Turret->Enabled = true;
}

void ThePlayer::NewGame()
{
	TheManagers.EM.SetTimer(TurretCooldownTimerID, 1.0f);
	TheManagers.EM.SetTimer(TurretHeatTimerID, 0.15f);
	TheManagers.EM.SetTimer(ShotTimerID, 0.125f);

	Lives = 4;
	NextNewLifeScore = 10000;
	Score = 0;
	GameOver = false;
	Reset();
}

void ThePlayer::PointTurret(float stickDirectionX, float stickDirectionY)
{
	Turret->RotationZ = atan2f(stickDirectionY, stickDirectionX) - RotationZ;
}

void ThePlayer::PointTurret(Vector3 mouseLocation)
{
	Turret->RotationZ = AngleFromVectorZ(mouseLocation) - RotationZ;
}

void ThePlayer::FireTurret()
{
	if (TurretOverHeat)
	{
		return;
	}

	if (TheManagers.EM.TimerElapsed(ShotTimerID))
	{
		for (auto& shot : Shots)
		{
			if (!shot->Enabled)
			{
				TheManagers.EM.ResetTimer(ShotTimerID);
				TurretHeat += 1;

				if (TurretHeat > TurretHeatMax)
				{
					TheManagers.EM.ResetTimer(TurretCooldownTimerID);
					TurretOverHeat = true;
				}

				Vector3 velocity = GetVelocityFromAngleZ(Turret->WorldRotation.z, 375.0f);
				shot->Spawn(Turret->WorldPosition, velocity, 2.15f);
				break;
			}
		}
	}

}

void ThePlayer::TurretTimers()
{
	if (TurretOverHeat)
	{
		if (TheManagers.EM.TimerElapsed(TurretCooldownTimerID))
		{
			TurretOverHeat = false;
			TurretHeat = 0;
		}
	}
	else
	{
		if (TheManagers.EM.TimerElapsed(TurretHeatTimerID))
		{
			TheManagers.EM.ResetTimer(TurretHeatTimerID);

			if (TurretHeat > 0)	TurretHeat -= 1;
		}
	}
}

void ThePlayer::CrosshairUpdate()
{
	Crosshair->X(GetMousePosition().x - WindowWidth);
	Crosshair->Y(GetMousePosition().y - WindowHeight);
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
}

void ThePlayer::ThrustOff()
{
	SetAccelerationToZero(0.45f);
	Flame->Enabled = false;
}

void ThePlayer::Gamepad()
{
	//Button B is 6 for Shield //Button A is 7 for Fire //Button Y is 8 for Hyperspace
	//Button X is 5	//Left bumper is 9 //Right bumper is 11 for Shield //Left Trigger is 10
	//Right Trigger is 12 for Thrust //Dpad Up is 1 for	//Dpad Down is 3 for
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

	PointTurret(GetGamepadAxisMovement(0, 2), GetGamepadAxisMovement(0, 3));

	//Right Trigger
	if (IsGamepadButtonDown(0, 12))
	{
	}
	else
	{
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
		Shield->Enabled = true;
	}
	else
	{
		Shield->Enabled = false;
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

	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_E))
	{
		Shield->Enabled = true;
	}
	else
	{
		Shield->Enabled = false;
	}

	PointTurret(Crosshair->Position);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		FireTurret();
	}
}