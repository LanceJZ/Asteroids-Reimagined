#include "TheAntiPlayer.h"

TheAntiPlayer::TheAntiPlayer()
{
	MovingTimerID = EM.AddTimer(1.0f);
	ShootingTimerID = EM.AddTimer(0.20f);
	FireRateTimerID = EM.AddTimer(0.10f);
}

TheAntiPlayer::~TheAntiPlayer()
{
}

bool TheAntiPlayer::Initialize()
{
	ThePlayerControls::Initialize();

	Enabled = false;

	Flame->ModelColor = { 200, 150, 0, 200 };
	PowerUpTimerAmount = 4.0f;

	return false;
}

bool TheAntiPlayer::BeginRun()
{
	ThePlayerControls::BeginRun();

	Shield->Scale = 1.63f;
	Shield->X(-7.0f);

	return false;
}

void TheAntiPlayer::Update(float deltaTime)
{
	ThePlayerControls::Update(deltaTime);

	switch (CurrentState)
	{
	case Idle:
		DoIdle();
		break;
	case ShootingAtPlayer:
		DoShootingAtPlayer();
		break;
	case ShootingAtRock:
		DoShootingAtRock();
		break;
	case Moveing:
		DoMoveing();
		break;
	}
}

void TheAntiPlayer::FixedUpdate(float deltaTime)
{
	ThePlayerControls::FixedUpdate(deltaTime);

	TurretTimers();
}

void TheAntiPlayer::Draw3D()
{
	ThePlayerControls::Draw3D();
}

void TheAntiPlayer::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void TheAntiPlayer::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void TheAntiPlayer::Destroy()
{
	Entity::Destroy();

}

void TheAntiPlayer::TurretTimers()
{
	ThePlayerControls::TurretTimers();

}

void TheAntiPlayer::DoIdle()
{
	if (Player->Enabled) CurrentState = ShootingAtPlayer;
}

void TheAntiPlayer::DoShootingAtPlayer()
{
	if (!Player->Enabled || Player->GetBeenHit()) CurrentState = Idle;

	PointTurret(PlayerPosition);

	if (EM.TimerElapsed(FireRateTimerID))
	{
		EM.ResetTimer(FireRateTimerID);
		FireTurret();
	}


	if (EM.TimerElapsed(ShootingTimerID))
	{
		CurrentState = Moveing;
		EM.ResetTimer(MovingTimerID);
	}
}

void TheAntiPlayer::DoChasingPlayer()
{
	if (!Player->Enabled || Player->GetBeenHit()) CurrentState = Idle;

	PlayerPosition = Player->Position;

	Vector3 target = PlayerPosition;
	if (X() > 0.0f) target.x = (float)WindowHalfWidth;
	else target.x = (float)-WindowHalfWidth;

	if (Y() > 0.0f) target.y = (float)WindowHalfHeight;
	else target.y = (float)-WindowHalfHeight;
	PlayerPosition = target;

	CurrentState = Moveing;
	EM.ResetTimer(MovingTimerID);
}

void TheAntiPlayer::DoShootingAtRock()
{
}

void TheAntiPlayer::DoShootingAtEnemy()
{
}

void TheAntiPlayer::DoMoveing()
{
	float direction = GetRotationTowardsTargetZ(Position, PlayerPosition, RotationZ, 1.0f);

	if (direction > 0.2f) RotateShip(1.0f);
	else if (direction < -0.2f) RotateShip(-1.0f);
	else RotateStop();

	ThrustOn(1.0f);

	if (EM.TimerElapsed(MovingTimerID))
	{
		CurrentState = ShootingAtPlayer;
		ThrustOff();
	}
}
