#include "TheAntiPlayer.h"

TheAntiPlayer::TheAntiPlayer()
{
	EM.AddEntity(AvoidBorder = DBG_NEW Entity());
	EM.AddEntity(ShieldBorder = DBG_NEW Entity());

	MovingTimerID = EM.AddTimer(1.0f);
	ShootingTimerID = EM.AddTimer(0.5f);
	FireRateTimerID = EM.AddTimer(0.10f);
	FireTimerID = EM.AddTimer(0.05f);
	IdelTimerID = EM.AddTimer(1.0f);
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

	ShieldBorder->Radius = Radius * 2.0f;
	ShieldBorder->EntityOnly = true;
	AvoidBorder->Radius = Radius * 4.0f;
	AvoidBorder->EntityOnly = true;

	return false;
}

bool TheAntiPlayer::BeginRun()
{
	ThePlayerControls::BeginRun();

	Shield->Scale = 1.63f;
	Shield->X(-7.0f);

	ShieldBorder->SetParent(*this);
	AvoidBorder->SetParent(*this);

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

	Turret->Spawn();
	CurrentState = Idle;
	EM.ResetTimer(IdelTimerID);
	DoChasingPlayer();
}

void TheAntiPlayer::Destroy()
{
	Entity::Destroy();

}

void TheAntiPlayer::TurretTimers()
{
	ThePlayerControls::TurretTimers();

}

void TheAntiPlayer::FireTurret()
{
	if (EM.TimerElapsed(FireRateTimerID))
	{
		for (auto& shot : Shots)
		{
			if (!shot->Enabled)
			{
				//PlaySound(FireSound);

				EM.ResetTimer(FireRateTimerID);

				if (GunOverCharge)
				{
					if (TurretHeat > 0)
					{
						GunOverCharge = false;
						Turret->ModelColor = WHITE;
					}
				}
				else
				{					
					if (TurretHeat > TurretHeatMax)
					{
						EM.ResetTimer(TurretCooldownTimerID);
						TurretOverheat = true;
						Turret->ModelColor = RED;
					}
					else TurretHeat += 5;
				}

				if (TurretOverheat) return;

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

void TheAntiPlayer::DoIdle()
{
	if (!Player->Enabled || Player->GetBeenHit()) EM.ResetTimer(IdelTimerID);

	ThrustOff();

	if (EM.TimerElapsed(IdelTimerID))
	{
		CurrentState = Moveing;
		EM.ResetTimer(MovingTimerID, M.GetRandomFloat(0.5f, 1.25f));
	}
}

void TheAntiPlayer::DoShootingAtPlayer()
{
	ThrustOff();

	if (!Player->Enabled || Player->GetBeenHit())
	{
		CurrentState = Idle;
		return;
	}


	if (EM.TimerElapsed(FireTimerID))
	{
		EM.ResetTimer(FireTimerID, M.GetRandomFloat(0.1f, 0.55f));

		if (AvoidRocksOrEnemies) PointTurret(RockOrEnemyPosition);
		else PointTurret(PlayerPosition);		
		
		FireTurret();
	}


	if (EM.TimerElapsed(ShootingTimerID))
	{
		CurrentState = Idle;
		EM.ResetTimer(IdelTimerID, M.GetRandomFloat(0.75f, 3.25f));
	}
}

void TheAntiPlayer::DoChasingPlayer()
{
	if (!Player->Enabled || Player->GetBeenHit()) CurrentState = Idle;

	PlayerPosition = Player->Position;

	Vector3 target = PlayerPosition;

	if (Vector3Distance(target, Position) > (float)WindowHalfWidth) target.x -= (float)WindowHalfWidth;
	
	if (Vector3Distance(target, Position) > (float)WindowHalfHeight) target.y -= (float)WindowHalfHeight;
	
	PlayerPosition = target;
}

void TheAntiPlayer::DoShootingAtRock()
{
}

void TheAntiPlayer::DoShootingAtEnemy()
{
}

void TheAntiPlayer::DoMoveing()
{
	float direction = 0;

	if (!Player->Enabled || Player->GetBeenHit())
	{
		CurrentState = Idle;
		RotateStop();
		ThrustOff();
		return;
	}

	if (AvoidRocksOrEnemies) direction = GetRotationTowardsTargetZ(Position, RockOrEnemyPosition, RotationZ, 1.0f);
	else direction = GetRotationTowardsTargetZ(Position, PlayerPosition, RotationZ, 1.0f);

	if (direction > 0.2f) RotateShip(1.0f);
	else if (direction < -0.2f) RotateShip(-1.0f);
	else RotateStop();

	ThrustOn(1.0f);

	if (EM.TimerElapsed(MovingTimerID))
	{
		RotateStop();
		ThrustOff();
		DoChasingPlayer();
		EM.ResetTimer(ShootingTimerID, M.GetRandomFloat(0.5f, 1.25f));
		CurrentState = ShootingAtPlayer;
	}
}
