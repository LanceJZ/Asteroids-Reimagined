#include "EnemyControl.h"

EnemyControl::EnemyControl()
{

}

EnemyControl::~EnemyControl()
{
}

void EnemyControl::SetPlayer(ThePlayer* player)
{
	Player = player;
}

bool EnemyControl::Initialize()
{

	return false;
}

bool EnemyControl::BeginRun()
{

	return false;
}

void EnemyControl::Update()
{
	Common::Update();

}

void EnemyControl::Reset()
{
}
