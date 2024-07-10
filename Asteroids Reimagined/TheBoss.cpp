#include "TheBoss.h"

TheBoss::TheBoss()
{
}

TheBoss::~TheBoss()
{
}

bool TheBoss::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool TheBoss::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheBoss::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void TheBoss::Draw3D()
{
	LineModel::Draw3D();
}

void TheBoss::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void TheBoss::Destroy()
{
	Entity::Destroy();

}
