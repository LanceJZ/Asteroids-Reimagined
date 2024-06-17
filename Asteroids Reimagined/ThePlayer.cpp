#include "ThePlayer.h"

ThePlayer::ThePlayer()
{
}

ThePlayer::~ThePlayer()
{
}

bool ThePlayer::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	//Scale = 3.5f;

	return false;
}

bool ThePlayer::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void ThePlayer::Input()
{
	LineModel::Input();

	Keyboard();

	if (IsGamepadAvailable(0))
	{
		Gamepad();
	}
}

void ThePlayer::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	CheckScreenEdge();
}

void ThePlayer::Draw()
{
	LineModel::Draw();

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
	SetAccelerationToMaxAtRotation(-(amount * 50.25f), 150.0f);
}

void ThePlayer::ThrustOff()
{
	SetAccelerationToZero(0.45f);
}

void ThePlayer::Hit()
{
	Acceleration = { 0 };
	Velocity = { 0 };
	Lives--;
	Enabled = false;
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
}

void ThePlayer::NewGame()
{
	Lives = 4;
	NextNewLifeScore = 10000;
	Score = 0;
	GameOver = false;
	Reset();
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
		ThrustOn(GetGamepadAxisMovement(0, 1));
	}
	else
	{
		ThrustOff();
	}

	//Right Stick
	if (GetGamepadAxisMovement(0, 2) > 0.25f) //Right
	{
	}
	else if (GetGamepadAxisMovement(0, 2) < -0.25f) //Left
	{
	}

	if (GetGamepadAxisMovement(0, 3) > 0.25f) //Down
	{
	}
	else if (GetGamepadAxisMovement(0, 3) < -0.25f) //Up
	{
	}


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
}

void ThePlayer::Keyboard()
{
	if (IsKeyDown(KEY_RIGHT))
	{
	}
	else if (IsKeyDown(KEY_LEFT))
	{
	}
	else
	{
	}

	if (IsKeyDown(KEY_UP))
	{
	}
	else
	{
	}

	if (IsKeyPressed(KEY_RIGHT_CONTROL) || IsKeyPressed(KEY_LEFT_CONTROL) ||
		IsKeyPressed(KEY_SPACE))
	{
	}

	if (IsKeyDown(KEY_DOWN))
	{
	}
	else
	{
	}
}