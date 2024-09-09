#include "TheMine.h"

TheMine::TheMine()
{
	LifeTimerID = Managers.EM.AddTimer(10.0f);
}

TheMine::~TheMine()
{
}

void TheMine::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void TheMine::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;

	SetSoundVolume(ExplodeSound, 1.25f);
}

void TheMine::SetParticleManager(ParticleManager* particles)
{
	Particles = particles;
}

bool TheMine::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool TheMine::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheMine::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (Managers.EM.TimerElapsed(LifeTimerID)) Destroy();

	CheckCollisions();
}

void TheMine::Draw3D()
{
	LineModel::Draw3D();
}

void TheMine::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	Managers.EM.ResetTimer(LifeTimerID);
}

void TheMine::Hit()
{
	PlaySound(ExplodeSound);

	Particles->SpawnLineParticles(Position, { 0.0f }, Radius * 0.25f, 50, 25, 1.0f, WHITE);

	Destroy();
}

void TheMine::Destroy()
{
	Entity::Destroy();

}

bool TheMine::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}
