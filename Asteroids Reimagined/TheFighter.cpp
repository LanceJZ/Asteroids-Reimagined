#include "TheFighter.h"

TheFighter::TheFighter()
{
}

TheFighter::~TheFighter()
{
}

void TheFighter::SetPlayer(ThePlayer* player)
{
	Player = player;
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

	if (Separated)
	{
		ChasePlayer();
		CheckScreenEdge();
	}
}

void TheFighter::Draw3D()
{
	LineModel::Draw3D();
}

void TheFighter::Separate()
{
	Separated = true;
	Position = GetWorldPosition();
	RotationZ = WorldRotation.z;
	IsChild = false;
}

void TheFighter::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void TheFighter::Hit()
{
	Entity::Hit();

}

void TheFighter::Destroy()
{
	Entity::Destroy();

}

void TheFighter::ChasePlayer()
{
	SetRotateVelocity(Player->Position, TurnSpeed, Speed);
}
