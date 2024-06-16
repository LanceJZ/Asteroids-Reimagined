#include "Shot.h"

Shot::Shot()
{
}

Shot::~Shot()
{
}

bool Shot::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	Radius = 6.0f;

	return false;
}

bool Shot::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void Shot::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (ScreenEdgeBoundY()) Destroy();
}

void Shot::Draw()
{
	LineModel::Draw();
}

void Shot::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	Enabled = true;
	Velocity.y = 300.0f;
}

void Shot::Spawn(Vector3 position, Vector3 velocity)
{
	Entity::Spawn(position);

	Enabled = true;
	Velocity = velocity;
}

void Shot::SpawnPlayerShot(Vector3 position)
{
	Entity::Spawn(position);

	Enabled = true;
	Velocity.y = -900.0f;
}

void Shot::Destroy()
{
	Entity::Destroy();

	Enabled = false;
}