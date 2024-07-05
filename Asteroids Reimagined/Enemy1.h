#pragma once
#include "Globals.h"
#include "Enemy.h"
#include "TheMissile.h"

class Enemy1 : public Enemy
{
public:
	enum Edge
	{
		Top,
		Bottom,
		Left,
		Right
	};

	Edge EdgeSpawnedFrom = Top;

	TheMissile* Missile = nullptr;

	Enemy1();
	virtual ~Enemy1();

	void SetPlayer(ThePlayer* player);
	void SetMissileModel(LineModelPoints model);

	void SetSpawnSound(Sound sound);
	void SetOnSound(Sound sound);
	void SetMissileExplodeSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();
	void Reset();

private:
	size_t FireMissileTimerID = 0;
	//int Score = 1250;
	float Speed = 133.666f;
	float TurnSpeed = 0.666f;
	float RotateMagnitude = PI / 2;

	Vector3 Destination = { 0.0f, 0.0f, 0.0f };

	Sound SpawnSound;
	Sound OnSound;

	bool CheckWentOffScreen();
	void DestinationTarget();
	void DestinationTop();
	void DestinationBottom();
	void DestinationLeft();
	void DestinationRight();
	void FireMissile();
	bool CheckCollisions();
};
