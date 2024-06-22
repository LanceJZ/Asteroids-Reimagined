#include "TheFighterPair.h"

TheFighterPair::TheFighterPair()
{
	for (int i = 0; i < 2; i++)
	{
		TheManagers.EM.AddLineModel(Fighters[i] = DBG_NEW TheFighter());
	}
}

TheFighterPair::~TheFighterPair()
{
}

void TheFighterPair::SetWedgeModel(LineModelPoints model)
{
	for (auto &fighter : Fighters)
	{
		fighter->SetModel(model);
	}

	Fighters[0]->Position.x = Fighters[0]->Radius * 0.875f;
	Fighters[1]->Position.x = -Fighters[1]->Radius * 0.875f;
}

bool TheFighterPair::Initialize(Utilities* utilities)
{
	Entity::Initialize(TheUtilities);

	return false;
}

bool TheFighterPair::BeginRun()
{
	Fighters[1]->RotationZ = PI;

	return false;
}

void TheFighterPair::Update(float deltaTime)
{
	Entity::Update(deltaTime);

}

void TheFighterPair::Draw3D()
{
	Entity::Draw3D();

}

void TheFighterPair::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}
