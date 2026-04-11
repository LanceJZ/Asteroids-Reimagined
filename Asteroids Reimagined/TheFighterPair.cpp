#include "TheFighterPair.h"

TheFighterPair::TheFighterPair()
{
	for (int i = 0; i < 2; i++)
	{
		EM.AddLineModel(Fighters[i] = DBG_NEW TheFighter());
	}
}

TheFighterPair::~TheFighterPair()
{
}

void TheFighterPair::SetPlayer(ThePlayer* player)
{
	Player = player;

	for (const auto &fighter : Fighters)
	{
		fighter->SetPlayer(player);
	}
}

void TheFighterPair::SetUFO(std::vector<Enemy*> ufos)
{
	UFORefs.clear();

	for (const auto& ufo : ufos)
	{
		UFORefs.push_back(ufo);
	}

	for (const auto& fighter : Fighters)
	{
		fighter->UFORefs.clear();

		for (const auto& ufo : ufos)
		{
			fighter->UFORefs.push_back(ufo);
		}
	}
}

void TheFighterPair::SetWedgeModel(std::vector<Vector3> model)
{
	for (const auto &fighter : Fighters)
	{
		fighter->SetModel(model);
	}
}

void TheFighterPair::SetExplodeSound(Sound sound)
{
	for (const auto &fighter : Fighters)
	{
		fighter->SetExplodeSound(sound);
	}

	ExplodeSound = sound;
}

bool TheFighterPair::Initialize()
{
	Enemy::Initialize();

	for (const auto &fighter : Fighters)
	{
		fighter->Initialize();
	}

	Enabled = false;

	return true;
}

bool TheFighterPair::BeginRun()
{
	Enemy::BeginRun();

	Points = 200;
	Speed = 100.0f;
	TurnSpeed = 0.5f;
	RotateMagnitude = PI / 2;

	return false;
}

void TheFighterPair::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (Separated)
	{
		CheckCollisions();
	}
}

void TheFighterPair::FixedUpdate(float deltaTime)
{
	Entity::FixedUpdate(deltaTime);

	if (Separated)
	{
		if (!Player->Enabled)
		{
			if (CheckUFOActive())
			{
				ChaseUFO();
				return;
			}
			else if (CheckEnemyActive())
			{
				ChaseEnemy();
				return;
			}

			if (LeaveScreen()) Destroy();

			return;
		}
		else
		{
			ChasePlayer();
		}

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
	Enemy::Draw3D();

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
	Enemy::Reset();

	for (const auto& fighter : Fighters)
	{
		fighter->Reset();
	}
}

void TheFighterPair::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

	Separated = false;
	NewWave = false;

	for (const auto &fighter : Fighters)
	{
		fighter->Reset();
		fighter->SetParent(*this);
		fighter->Wave = Wave;
		//fighter->Spawn(position);
	}

	Fighters[0]->RotationZ = 0.0f;
	Fighters[1]->RotationZ = PI;

	float offset = 0.78f;
	//Fighters[0]->Position.x = Fighters[0]->Radius * offset;
	//Fighters[0]->Position.y = 0.0f;
	//Fighters[1]->Position.x = -Fighters[1]->Radius * offset;
	//Fighters[1]->Position.y = 0.0f;
	Fighters[0]->Spawn({ Fighters[0]->Radius * offset, 0.0f, 0.0f });
	Fighters[1]->Spawn({ -Fighters[1]->Radius * offset, 0.0f, 0.0f });
}

void TheFighterPair::Hit()
{
	Enemy::Hit();

	for (const auto &fighter : Fighters)
	{
		fighter->Separate();
		fighter->RemoveParent(this);
	}

	ClearParents();
	Destroy();
}

void TheFighterPair::Destroy()
{
	Enemy::Destroy();

}

bool TheFighterPair::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}
