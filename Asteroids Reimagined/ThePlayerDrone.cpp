#include "ThePlayerDrone.h"

ThePlayerDrone::ThePlayerDrone()
{
}

ThePlayerDrone::~ThePlayerDrone()
{
}

void ThePlayerDrone::SetPlayer(ThePlayer* player)
{
	Player = player;
}

bool ThePlayerDrone::Initialize()
{
	LineModel::Initialize();

	return false;
}

bool ThePlayerDrone::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void ThePlayerDrone::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void ThePlayerDrone::Draw3D()
{
	LineModel::Draw3D();
}

void ThePlayerDrone::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void ThePlayerDrone::Destroy()
{
	Entity::Destroy();

}
