#include "TheMine.h"

TheMine::TheMine()
{
	LifeTimerID = TheManagers.EM.AddTimer(10.0f);
}

TheMine::~TheMine()
{
}

void TheMine::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void TheMine::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;

	SetSoundVolume(ExplodeSound, 1.25f);
}

bool TheMine::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool TheMine::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheMine::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (TheManagers.EM.TimerElapsed(LifeTimerID)) Destroy();

	CheckCollisions();
}

void TheMine::Draw3D()
{
	LineModel::Draw3D();
}

void TheMine::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	TheManagers.EM.ResetTimer(LifeTimerID);
}

void TheMine::Hit()
{
	PlaySound(ExplodeSound);

	Destroy();
}

void TheMine::Destroy()
{
	Entity::Destroy();

}

void TheMine::CheckCollisions()
{
	for (auto& playerShot : Player->Shots)
	{
		if (playerShot->Enabled && playerShot->CirclesIntersect(*this))
		{
			playerShot->Destroy();
			Hit();
			break;
		}
	}

	if (Player->Enabled && CirclesIntersect(*Player))
	{
		Player->Hit(Position, Velocity);

		if (!Player->Shield->Enabled) Destroy();
	}
}
