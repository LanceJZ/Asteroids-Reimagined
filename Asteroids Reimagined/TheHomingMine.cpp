#include "TheHomingMine.h"

TheHomingMine::TheHomingMine()
{
	LifeTimerID = Managers.EM.AddTimer(90.0f);
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

}

void TheHomingMine::FixedUpdate(float deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	if (Managers.EM.TimerElapsed(LifeTimerID)) Destroy();

	if (Target != nullptr)
	{
		if (!Target->Enabled) LostEnemy();
		else ChaseEnemy(Target->GetWorldPosition());
	}
	else LostEnemy();
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

void TheHomingMine::Hit()
{
	PlaySound(ExplodeSound);

	Particles->SpawnLineParticles(Position, { 0.0f }, Radius * 0.25f, 50, 25, 1.0f, WHITE);

	Destroy();
}

void TheHomingMine::Destroy()
{
	LineModel::Destroy();

}

void TheHomingMine::ChaseEnemy(Vector3 enemyPosition)
{
	RotationZ = GetAngleFromVectors(enemyPosition);
	Velocity = GetVelocityFromAngleZ(166.666f);
}

void TheHomingMine::ChaseEnemy(Entity* enemy)
{
	Target = enemy;
}

void TheHomingMine::LostEnemy()
{
	Velocity = { 0.0f, 0.0f, 0.0f };
	RotationZ = 0.0f;

	Target = nullptr;
}