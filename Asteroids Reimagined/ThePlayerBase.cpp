#include "ThePlayerBase.h"

ThePlayerBase::ThePlayerBase()
{
	EM.AddLineModel(Flame = DBG_NEW LineModel());
}

ThePlayerBase::~ThePlayerBase()
{
}

void ThePlayerBase::SetShotModel(std::vector<Vector3> model)
{
	for (int i = 0; i < MagazineSize; i++)
	{
		Shots.push_back(DBG_NEW Shot());
		EM.AddLineModel(Shots.back());
	}

	std::vector<Vector3> ShotModel;

	for (const auto& shot : Shots)
	{
		shot->SetModel(model);
	}
}

void ThePlayerBase::SetFlameModel(std::vector<Vector3> model)
{
	Flame->SetModel(model);
}

void ThePlayerBase::SetFireSound(Sound sound)
{
	FireSound = sound;
}

void ThePlayerBase::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
}

bool ThePlayerBase::Initialize()
{
	LineModel::Initialize();

	return false;
}

bool ThePlayerBase::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void ThePlayerBase::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void ThePlayerBase::Draw3D()
{
	LineModel::Draw3D();
}

void ThePlayerBase::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void ThePlayerBase::Destroy()
{
	Entity::Destroy();

}
