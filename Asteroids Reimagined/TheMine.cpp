#include "TheMine.h"

TheMine::TheMine()
{
	LifeTimerID = TheManagers.EM.AddTimer(10.0f);
}

TheMine::~TheMine()
{
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

}

void TheMine::Draw3D()
{
	LineModel::Draw3D();
}

void TheMine::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	TheManagers.EM.ResetTimer(LifeTimerID);
}

void TheMine::Destroy()
{
	Entity::Destroy();

}
