#pragma once
#include "Globals.h"
#include "Enemy.h"
#include "TheMine.h"

class Enemy2 : public Enemy
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

	std::vector<TheMine*> Mines;

	Enemy2();
	virtual ~Enemy2();

	void SetMineModel(LineModelPoints model);
	void SetSpawnSound(Sound sound);
	void SetOnSound(Sound sound);
	void SetMineExplodeSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn();
	void Spawn(Vector3 position);
	void Hit();
	void Destroy();
	void Reset();

private:
	size_t LayMineTimerID = 0;

	float Speed = 75.0f;
	float TurnSpeed = 0.4666f;
	float RotateMagnitude = PI / 2;
	float MineDropTimeAmount = 0.0f;

	Vector3 Destination = { 0.0f, 0.0f, 0.0f };

	Sound MineExplodeSound = {};

	LineModelPoints MineModel = {};

	bool CheckWentOffScreen();
	void DestinationTarget();
	void DestinationTop();
	void DestinationBottom();
	void DestinationLeft();
	void DestinationRight();
	void DropMine();
	bool CheckCollisions();
};
