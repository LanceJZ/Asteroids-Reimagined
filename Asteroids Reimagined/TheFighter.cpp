#include "TheFighter.h"

TheFighter::TheFighter()
{
}

TheFighter::~TheFighter()
{
}

bool TheFighter::Initialize(Utilities* utilities)
{
	LineModel::Initialize(TheUtilities);

	return false;
}

bool TheFighter::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheFighter::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void TheFighter::Draw3D()
{
	LineModel::Draw3D();
}

void TheFighter::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void TheFighter::Destroy()
{
	Entity::Destroy();

}
