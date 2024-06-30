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

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:
	size_t LayMineTimerID = 0;

	int Score = 250;
	float Speed = 75.0f;
	float TurnSpeed = 0.4666f;
	float RotateMagnitude = PI / 2;

	Vector3 Destination = { 0.0f, 0.0f, 0.0f };

	LineModelPoints MineModel = {};

	bool CheckWentOffScreen();
	void DestinationTarget();
	void DestinationTop();
	void DestinationBottom();
	void DestinationLeft();
	void DestinationRight();
	void DropMine();
};
