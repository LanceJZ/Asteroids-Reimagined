#include "TheBoss.h"

TheBoss::TheBoss()
{
	TheManagers.EM.AddEntity(Shield = DBG_NEW LineModel());
}

TheBoss::~TheBoss()
{
}

void TheBoss::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void TheBoss::SetShieldModel(LineModelPoints model)
{
	Shield->SetModel(model);
}

void TheBoss::SetTurretModel(LineModelPoints model)
{
	TurretModel = model;
}

void TheBoss::SetMissileModel(LineModelPoints model)
{

	MissileModel = model;
}

void TheBoss::SetShotModel(LineModelPoints model)
{

	ShotModel = model;
}

void TheBoss::SetMineModel(LineModelPoints model)
{

	MineModel = model;
}

bool TheBoss::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool TheBoss::BeginRun()
{
	LineModel::BeginRun();

	Shield->SetParent(*this);

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
