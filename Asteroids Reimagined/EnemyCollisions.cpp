#include "EnemyCollisions.h"

EnemyCollisions::EnemyCollisions()
{

}

EnemyCollisions::~EnemyCollisions()
{
}

void EnemyCollisions::SetPlayer(ThePlayer* player)
{
	player = Player;
}

bool EnemyCollisions::Initialize(Utilities* utilities)
{
	Common::Initialize(TheUtilities);

	return false;
}

bool EnemyCollisions::BeginRun()
{

	return false;
}

void EnemyCollisions::Update()
{
	Common::Update();

}
