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

void TheFighterPair::SetUFO(Enemy* ufo[2])
{
	for (int i = 0; i < 2; i++)
	{
		UFOs[i] = ufo[i];
	}

	for (const auto &fighter : Fighters)
	{
		fighter->SetUFO(ufo);
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

	//ExplodeSound = sound;
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
	Entity::Update(deltaTime);

	if (Separated)
	{
		if (Fighters[0]->Player->Enabled)
		{
			ChasePlayer();
		}
		else if (Fighters[0]->UFOs[0]->Enabled || Fighters[0]->UFOs[1]->Enabled)
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

	//Velocity = { 0.0f, 0.0f, 0.0f };
	//RotationVelocityZ = 0.0f;
	//Destroy();
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
	Entity::Hit();

	if (!Fighters[0]->Player->GameOver) PlaySound(Fighters[0]->ExplodeSound);

	for (const auto &fighter : Fighters)
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
	SetRotateVelocity(Fighters[0]->Player->Position, TurnSpeed, Speed);
}

void TheFighterPair::ChaseUFO()
{
	Enemy::ChaseUFO();

	//if (UFOs[0]->Enabled && UFOs[1]->Enabled)
	//{
	//	UFOs[0]->Distance =	Vector3Distance(UFOs[0]->Position, Position);
	//	UFOs[1]->Distance =	Vector3Distance(UFOs[1]->Position, Position);

	//	if (UFOs[0]->Distance <	UFOs[1]->Distance && UFOs[0]->Enabled)
	//	{
	//		SetRotateVelocity(UFOs[0]->Position, TurnSpeed, Speed);
	//	}
	//	else if (UFOs[1]->Distance < UFOs[0]->Distance && UFOs[1]->Enabled)
	//	{
	//		SetRotateVelocity(UFOs[1]->Position, TurnSpeed, Speed);
	//	}
	//}
	//else if (UFOs[0]->Enabled)
	//{
	//	SetRotateVelocity(UFOs[0]->Position, TurnSpeed, Speed);
	//}
	//else if (UFOs[1]->Enabled)
	//{
	//	SetRotateVelocity(UFOs[1]->Position, TurnSpeed, Speed);
	//}
}

void TheFighterPair::LeaveScreen()
{
	LeavePlay(TurnSpeed, Speed);

	if (OffScreen())
	{
		Reset();
		Destroy();

		for (const auto &fighter : Fighters)
		{
			fighter->Reset();
			fighter->Destroy();
		}
	}
}

bool TheFighterPair::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;

	//if (Player->Enabled && CirclesIntersect(*Player))
	//{
	//	Player->Hit(Position, Velocity);

	//	if (!Player->Shield->Enabled)
	//	{
	//		Hit();
	//		Player->ScoreUpdate(Points);
	//	}

	//	return;
	//}

	//for (const auto& shot : Player->Shots)
	//{
	//	if (shot->Enabled && CirclesIntersect(*shot))
	//	{
	//		shot->Destroy();
	//		Hit();
	//		Player->ScoreUpdate(Points);

	//		return;
	//	}
	//}

	//for (const auto& shot : Player->DoubleShots)
	//{
	//	if (shot->Enabled && CirclesIntersect(*shot))
	//	{
	//		shot->Destroy();
	//		Hit();
	//		Player->ScoreUpdate(Points);

	//		return;
	//	}
	//}

	//for (const auto& bigShot : Player->BigShots)
	//{
	//	if (bigShot->Enabled && CirclesIntersect(*bigShot))
	//	{
	//		bigShot->HitPoints -= 50;

	//		if (bigShot->HitPoints <= 0) bigShot->Destroy();

	//		Hit();
	//		Player->ScoreUpdate(Points);

	//		return;
	//	}
	//}

	//for (const auto& mine : Player->Mines)
	//{
	//	if (mine->Enabled && CirclesIntersect(*mine))
	//	{
	//		mine->Destroy();
	//		Hit();
	//		Player->ScoreUpdate(Points);

	//		return;
	//	}
	//}

	//for (const auto& ufo : UFOs)
	//{
	//	if (ufo->Enabled && CirclesIntersect(*ufo))
	//	{
	//		ufo->Hit();
	//		Hit();
	//		return;
	//	}

	//	for (const auto& shot : ufo->Shots)
	//	{
	//		if (shot->Enabled && CirclesIntersect(*shot))
	//		{
	//			shot->Destroy();
	//			Hit();
	//			return;
	//		}
	//	}
	//}
}
