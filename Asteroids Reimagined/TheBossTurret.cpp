#include "TheBossTurret.h"

TheBossTurret::TheBossTurret()
{
}

TheBossTurret::~TheBossTurret()
{
}

bool TheBossTurret::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool TheBossTurret::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheBossTurret::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void TheBossTurret::Draw3D()
{
	LineModel::Draw3D();
}

void TheBossTurret::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void TheBossTurret::Destroy()
{
	Entity::Destroy();

}
