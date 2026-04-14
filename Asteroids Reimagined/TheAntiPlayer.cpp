#include "TheAntiPlayer.h"

TheAntiPlayer::TheAntiPlayer()
{
}

TheAntiPlayer::~TheAntiPlayer()
{
}

bool TheAntiPlayer::Initialize()
{
	LineModel::Initialize();

	return false;
}

bool TheAntiPlayer::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheAntiPlayer::SetPlayer(ThePlayer* player)
{
	Enemy::SetPlayer(player);
}

void TheAntiPlayer::SetShotModel(std::vector<Vector3> model)
{
	Enemy::SetShotModel(model);
}

void TheAntiPlayer::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void TheAntiPlayer::Draw3D()
{
	LineModel::Draw3D();
}

void TheAntiPlayer::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void TheAntiPlayer::Destroy()
{
	Entity::Destroy();

}
