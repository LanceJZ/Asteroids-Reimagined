#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheUFO.h"

class TheFighter : public LineModel
{
public:
	TheFighter();
	virtual ~TheFighter();

	bool NewWave = false;

	void SetPlayer(ThePlayer* player);
	void SetUFO(TheUFO* ufo[2]);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Separate();
	void Reset();
	void Destroy();

	void Spawn(Vector3 position);

private:
	bool Separated = false;
	int Points = 200;
	float Speed = 150.0f;
	float TurnSpeed = 0.45f;
	float RotateMagnitude = PI / 2;

	ThePlayer* Player = nullptr;
	TheUFO* UFOs[2] = { nullptr };

	void ChasePlayer();
	void ChaseUFO();
	void LeaveScreen();
	void CheckCollisions();
	void Hit();
};