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

	std::vector<TheMissile*> Missiles;

	Enemy1();
	virtual ~Enemy1();

	void SetPlayer(ThePlayer* player);
	void SetMissileModel(LineModelPoints model);

	void SetSpawnSound(Sound sound);
	void SetOnSound(Sound sound);
	void SetMissileOnSound(Sound sound);
	void SetMissileExplodeSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(double deltaTime);
	void FixedUpdate(double deltaTime);
	void Draw3D();

	void Spawn();
	void Spawn(Vector3 position);
	void Hit();
	void Destroy();
	void Reset();

private:
	size_t FireMissileTimerID = 0;
	int MissilesFired = 0;

	float MissileFireTimerAmount = 0.0f;
	float Speed = 133.666f;
	float TurnSpeed = 0.666f;
	float RotateMagnitude = PI / 2;

	Sound MissileOnSound = {};
	Sound MissileExplodeSound = {};

	LineModelPoints MissileModel = {};

	bool CheckWentOffScreen();
	void DestinationTarget();
	void FireMissile();
	void FireShot();
	bool CheckCollisions();
};
