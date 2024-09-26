#include "TheFighterPair.h"

TheFighterPair::TheFighterPair()
{
	for (int i = 0; i < 2; i++)
	{
		Managers.EM.AddLineModel(Fighters[i] = DBG_NEW TheFighter());
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

void TheFighterPair::SetEnemies(Enemy* enemyOne, Enemy* enemyTwo)
{
	EnemyOne = enemyOne;
	EnemyTwo = enemyTwo;

	for (const auto &fighter : Fighters)
	{
		fighter->SetEnemies(enemyOne, enemyTwo);
	}
}

void TheFighterPair::SetWedgeModel(LineModelPoints model)
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

void TheFighterPair::SetParticleManager(ParticleManager* particleManager)
{
	for (const auto& fighter : Fighters)
	{
		fighter->SetParticleManager(particleManager);
	}
}

bool TheFighterPair::Initialize(Utilities* utilities)
{
	Entity::Initialize(TheUtilities);

	for (const auto &fighterPair : Fighters)
	{
		fighterPair->Initialize(TheUtilities);
	}

	Enabled = false;

	return true;
}

bool TheFighterPair::BeginRun()
{
	Entity::BeginRun();

	Points = 200;
	Speed = 100.0f;
	TurnSpeed = 0.5f;
	RotateMagnitude = PI / 2;

	return false;
}

void TheFighterPair::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	if (Separated)
	{
		CheckCollisions();

		if (CheckUFOActive())
		{
			ChaseUFO();
		}
		else if (EnemyOne->Enabled || EnemyTwo->Enabled)
		{
			ChaseEnemy();
		}
		else if (!Player->Enabled)
		{
			//if (LeaveScreen()) Destroy();

			LeaveScreen();

			return;
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
	Enemy::Reset();

	for (const auto& fighter : Fighters)
	{
		fighter->Reset();
	}
}

void TheFighterPair::Spawn(Vector3 position)
{
	Enabled = true;
	Separated = false;
	NewWave = false;

	for (const auto &fighter : Fighters)
	{
		fighter->Reset();
		fighter->SetParent(*this);
		fighter->Spawn(position);
	}

	Fighters[0]->RotationZ = 0.0f;
	Fighters[1]->RotationZ = PI;

	float offset = 0.78f;
	Fighters[0]->Position.x = Fighters[0]->Radius * offset;
	Fighters[0]->Position.y = 0.0f;
	Fighters[1]->Position.x = -Fighters[1]->Radius * offset;
	Fighters[1]->Position.y = 0.0f;
}

void TheFighterPair::Hit()
{
	Enemy::Hit();

	if (!Player->GameOver) PlaySound(ExplodeSound);

	for (const auto &fighter : Fighters)
	{
		fighter->Separate();
		fighter->RemoveParent(this);
	}

	Destroy();
}

void TheFighterPair::Destroy()
{
	Enemy::Destroy();

	ClearParents();
}

void TheFighterPair::ChasePlayer()
{
	SetRotateVelocity(Player->Position, TurnSpeed, Speed);
}

bool TheFighterPair::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}
