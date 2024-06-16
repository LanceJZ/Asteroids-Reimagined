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

bool EnemyControl::Initialize(Utilities* utilities)
{
	Common::Initialize(utilities);

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
