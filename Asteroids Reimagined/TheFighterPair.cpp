#include "TheFighterPair.h"

TheFighterPair::TheFighterPair()
{
	for (int i = 0; i < 2; i++)
	{
		TheManagers.EM.AddLineModel(Fighters[i] = DBG_NEW TheFighter());
	}
}

TheFighterPair::~TheFighterPair()
{
}

void TheFighterPair::SetPlayer(ThePlayer* player)
{
	Player = player;

	for (auto &fighter : Fighters)
	{
		fighter->SetPlayer(player);
	}
}

void TheFighterPair::SetUFO(TheUFO* ufo[2])
{
	for (int i = 0; i < 2; i++)
	{
		UFOs[i] = ufo[i];
	}

	for (auto &fighter : Fighters)
	{
		fighter->SetUFO(ufo);
	}
}

void TheFighterPair::SetWedgeModel(LineModelPoints model)
{
	for (auto &fighter : Fighters)
	{
		fighter->SetModel(model);
	}
}

bool TheFighterPair::Initialize(Utilities* utilities)
{
	Entity::Initialize(TheUtilities);

	for (auto &fighterPair : Fighters)
	{
		fighterPair->Initialize(TheUtilities);
	}

	Enabled = false;

	return true;
}

bool TheFighterPair::BeginRun()
{

	return false;
}

void TheFighterPair::Update(float deltaTime)
{
	Entity::Update(deltaTime);

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

void TheFighterPair::Draw3D()
{
	Entity::Draw3D();

}

void TheFighterPair::Separate()
{
	Separated = true;
	Position = GetWorldPosition();
	Radius = 16.0f;
	IsChild = false;
}

void TheFighterPair::Reset()
{
	Velocity = { 0.0f, 0.0f, 0.0f };
	RotationVelocityZ = 0.0f;
	Destroy();
}

void TheFighterPair::Spawn(Vector3 position)
{
	Enabled = true;
	Separated = false;
	NewWave = false;

	for (auto &fighter : Fighters)
	{
		fighter->Reset();
		fighter->SetParent(*this);
		fighter->Spawn(position);
	}

	Fighters[0]->RotationZ = 0.0f;
	Fighters[1]->RotationZ = PI;

	float offset = 0.875f;
	Fighters[0]->Position.x = Fighters[0]->Radius * offset;
	Fighters[0]->Position.y = 0.0f;
	Fighters[1]->Position.x = -Fighters[1]->Radius * offset;
	Fighters[1]->Position.y = 0.0f;
}

void TheFighterPair::Hit()
{
	Entity::Hit();

	for (auto &fighter : Fighters)
	{
		fighter->Separate();
		fighter->RemoveParent(this);
	}

	Reset();
	Destroy();
}

void TheFighterPair::Destroy()
{
	Entity::Destroy();

	ClearParents();
}

void TheFighterPair::ChasePlayer()
{
	SetRotateVelocity(Player->Position, TurnSpeed, Speed);
}

void TheFighterPair::ChaseUFO()
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

void TheFighterPair::LeaveScreen()
{
	LeavePlay(TurnSpeed, Speed);

	if (OffScreen())
	{
		Reset();
		Destroy();

		for (auto &fighter : Fighters)
		{
			fighter->Reset();
			fighter->Destroy();
		}
	}
}

void TheFighterPair::CheckCollisions()
{
	if (Player->Enabled && CirclesIntersect(*Player))
	{
		Player->Hit(Position, Velocity);

		if (!Player->Shield->Enabled)
		{
			Hit();
			Destroy();
			Player->ScoreUpdate(Points);
		}

		return;
	}

	for (auto& shot : Player->Shots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			Destroy();
			Player->ScoreUpdate(Points);

			return;
		}
	}

	for (auto& ufo : UFOs)
	{
		if (ufo->Enabled && CirclesIntersect(*ufo))
		{
			ufo->Hit();
			ufo->Destroy();
			Hit();
			Destroy();
			return;
		}

		for (auto& shot : ufo->Shots)
		{
			if (shot->Enabled && CirclesIntersect(*shot))
			{
				shot->Destroy();
				ufo->Hit();
				ufo->Destroy();
				Hit();
				Destroy();
				return;
			}
		}
	}
}
