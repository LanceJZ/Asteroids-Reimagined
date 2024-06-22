#pragma once
#include "Globals.h"
#include "ThePlayer.h"

class TheFighter : public LineModel
{
public:
	TheFighter();
	virtual ~TheFighter();

	void SetPlayer(ThePlayer* player);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Separate();

	void Spawn(Vector3 position);
	void Hit();
	void Destroy();

private:
	bool Separated = false;
	int Score = 200;
	float Speed = 150.0f;
	float TurnSpeed = 0.45f;
	float RotateMagnitude = PI / 2;

	ThePlayer* Player;

	void ChasePlayer();

};