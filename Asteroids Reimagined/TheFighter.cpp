#include "TheFighter.h"

TheFighter::TheFighter()
{
}

TheFighter::~TheFighter()
{
}

void TheFighter::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void TheFighter::SetUFO(TheUFO* ufo[2])
{
	for (int i = 0; i < 2; i++)
	{
		UFOs[i] = ufo[i];
	}
}

void TheFighter::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
}

bool TheFighter::Initialize(Utilities* utilities)
{
	LineModel::Initialize(TheUtilities);

	Enabled = false;

	return false;
}

bool TheFighter::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheFighter::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (Separated)
	{
		if (Player->Enabled)
		{
			ChasePlayer();
		}
		else if (UFOs[0]->Enabled || UFOs[1]->Enabled)
		{
			ChaseUFO();
		}
		else
		{
			LeaveScreen();
		}

		CheckCollisions();

		if (NewWave)
		{
			LeaveScreen();
		}
		else
		{
			CheckScreenEdge();
		}
	}
}

void TheFighter::Draw3D()
{
	LineModel::Draw3D();
}

void TheFighter::Separate()
{
	Separated = true;
	Position = GetWorldPosition();
	RotationZ = WorldRotation.z;
	IsChild = false;
}

void TheFighter::Reset()
{
	Velocity = { 0.0f, 0.0f, 0.0f };
	RotationVelocityZ = 0.0f;
	Destroy();
}

void TheFighter::Spawn(Vector3 position)
{
	Enabled = true;
	Separated = false;
	NewWave = false;
}

void TheFighter::Hit()
{
	Entity::Hit();

	if (!Player->GameOver) PlaySound(ExplodeSound);

	Reset();
}

void TheFighter::Destroy()
{
	Entity::Destroy();

	ClearParents();
}

void TheFighter::ChasePlayer()
{
	SetRotateVelocity(Player->Position, TurnSpeed, Speed);
}

void TheFighter::ChaseUFO()
{
	if (UFOs[0]->Enabled && UFOs[1]->Enabled)
	{
		UFOs[0]->Distance = Vector3Distance(UFOs[0]->Position, Position);
		UFOs[1]->Distance = Vector3Distance(UFOs[1]->Position, Position);

		if (UFOs[0]->Distance < UFOs[1]->Distance && UFOs[0]->Enabled)
		{
			SetRotateVelocity(UFOs[0]->Position, TurnSpeed, Speed);
		}
		else if (UFOs[1]->Distance < UFOs[0]->Distance && UFOs[1]->Enabled)
		{
			SetRotateVelocity(UFOs[1]->Position, TurnSpeed, Speed);
		}
	}
	else if (UFOs[0]->Enabled)
	{
		SetRotateVelocity(UFOs[0]->Position, TurnSpeed, Speed);
	}
	else if (UFOs[1]->Enabled)
	{
		SetRotateVelocity(UFOs[1]->Position, TurnSpeed, Speed);
	}
}

void TheFighter::LeaveScreen()
{
	LeavePlay(TurnSpeed, Speed);

	if (OffScreen())
	{
		Reset();
		Destroy();
	}
}

void TheFighter::CheckCollisions()
{
	if (Player->Enabled && CirclesIntersect(*Player))
	{
		Player->Hit(Position, Velocity);

		if (!Player->Shield->Enabled)
		{
			Hit();
			Player->ScoreUpdate(Points);
		}

		return;
	}

	for (const auto& shot : Player->Shots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			Player->ScoreUpdate(Points);

			return;
		}
	}

	for (const auto& ufo : UFOs)
	{
		if (ufo->Enabled && CirclesIntersect(*ufo))
		{
			ufo->Hit();
			Hit();
			return;
		}

		for (const auto& shot : ufo->Shots)
		{
			if (shot->Enabled && CirclesIntersect(*shot))
			{
				shot->Destroy();
				Hit();
				return;
			}
		}
	}
}
