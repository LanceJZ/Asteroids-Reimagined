#include "TheAntiPlayer.h"

TheAntiPlayer::TheAntiPlayer()
{
}

TheAntiPlayer::~TheAntiPlayer()
{
}

void TheAntiPlayer::SetStarShotModel(std::vector<Vector3> model)
{
	StarShotModel = model;
}

bool TheAntiPlayer::Initialize()
{
	LineModel::Initialize();

	Enabled = false;

	return false;
}

bool TheAntiPlayer::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheAntiPlayer::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void TheAntiPlayer::Draw3D()
{
	LineModel::Draw3D();
}

void TheAntiPlayer::SetPlayer(Entity* player)
{
	Player = player;
}

void TheAntiPlayer::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void TheAntiPlayer::Destroy()
{
	Entity::Destroy();

}
