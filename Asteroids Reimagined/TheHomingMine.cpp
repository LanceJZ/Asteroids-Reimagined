#include "TheHomingMine.h"

TheHomingMine::TheHomingMine()
{
	LifeTimerID = Managers.EM.AddTimer(30.0f);
}

TheHomingMine::~TheHomingMine()
{
}

void TheHomingMine::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
}

void TheHomingMine::SetParticleManager(ParticleManager* particles)
{
	Particles = particles;
}

bool TheHomingMine::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool TheHomingMine::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheHomingMine::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	CheckIsEnemyClose();


	if (Managers.EM.TimerElapsed(LifeTimerID)) Destroy();
}

void TheHomingMine::Draw3D()
{
	LineModel::Draw3D();
}

void TheHomingMine::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	Managers.EM.ResetTimer(LifeTimerID, 30.0f);
}

void TheHomingMine::Destroy()
{
	LineModel::Destroy();

}

void TheHomingMine::CheckIsEnemyClose()
{
	bool enemyToChase = false;
	float distance = 600.0f;

	//for (const auto& ufo : Enemies->UFOs)
	//{
	//	if (!ufo->Enabled) continue;

	//	float ufoDistance = Vector3Distance(ufo->Position, Position);

	//	if (ufoDistance < distance)
	//	{
	//		distance = ufoDistance;
	//		enemyToChase = true;
	//		EnemyPosition = ufo->Position;
	//		EnemyRadius = ufo->Radius;
	//	}
	//}

	//if (Enemies->EnemyOne->Enabled)
	//{
	//	float enemyOneDistance = Vector3Distance(Enemies->EnemyOne->Position, Position);

	//	if (enemyOneDistance < distance)
	//	{
	//		distance = enemyOneDistance;
	//		enemyToChase = true;
	//		EnemyPosition = Enemies->EnemyOne->Position;
	//		EnemyRadius = Enemies->EnemyOne->Radius;
	//	}
	//}

	//if (Enemies->EnemyTwo->Enabled)
	//{
	//	float enemyTwoDistance = Vector3Distance(Enemies->EnemyTwo->Position, Position);

	//	if (enemyTwoDistance < distance)
	//	{
	//		distance = enemyTwoDistance;
	//		enemyToChase = true;
	//		EnemyPosition = Enemies->EnemyTwo->Position;
	//		EnemyRadius = Enemies->EnemyTwo->Radius;
	//	}
	//}

	//for (const auto& fighterPair : Enemies->DeathStar->FighterPairs)
	//{
	//	if (!fighterPair->Enabled) continue;

	//	float fighterDistance = Vector3Distance(fighterPair->Position, Position);

	//	if (fighterDistance < distance)
	//	{
	//		distance = fighterDistance;
	//		enemyToChase = true;
	//		EnemyPosition = fighterPair->Position;
	//		EnemyRadius = fighterPair->Radius;
	//	}

	//	for (const auto& fighter : fighterPair->Fighters)
	//	{
	//		if (!fighter->Enabled) continue;

	//		float fighterDistance = Vector3Distance(fighter->Position, Position);

	//		if (fighterDistance < distance)
	//		{
	//			distance = fighterDistance;
	//			enemyToChase = true;
	//			EnemyPosition = fighter->Position;
	//			EnemyRadius = fighter->Radius;
	//		}
	//	}
	//}
}

void TheHomingMine::ChaseEnemy(Vector3 enemyPosition)
{
	RotationZ = GetAngleFromVectors(enemyPosition);
	Velocity = VelocityFromAngleZ(66.666f);
}

void TheHomingMine::LostEnemy()
{
	Velocity = { 0.0f, 0.0f, 0.0f };
}

void TheHomingMine::CheckCollision()
{
	//The enemies will need to check this.
}
