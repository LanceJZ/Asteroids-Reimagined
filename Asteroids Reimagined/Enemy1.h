#pragma once
#include "Globals.h"
#include "Enemy.h"

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

	Enemy1();
	virtual ~Enemy1();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:
	int Score = 250;
	float Speed = 133.666f;
	float TurnSpeed = 0.666f;
	float RotateMagnitude = PI / 2;

	Vector3 Destination = { 0.0f, 0.0f, 0.0f };

	bool CheckWentOffScreen();
	void DestinationTarget();
	void DestinationTop();
	void DestinationBottom();
	void DestinationLeft();
	void DestinationRight();
};
