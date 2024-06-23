#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheFighter.h"
#include "TheUFO.h"

class TheFighterPair : public Entity
{
public:
	TheFighterPair();
	virtual ~TheFighterPair();

	TheFighter* Fighters[2] = { nullptr };

	void SetPlayer(ThePlayer* player);
	void SetUFO(TheUFO* ufo[2]);
	void SetWedgeModel(LineModelPoints model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Separate();

	void Spawn(Vector3 position);

private:
	int TheScore = 200;
	float Speed = 100.0f;
	float TurnSpeed = 0.5f;
	float RotateMagnitude = PI / 2;

	ThePlayer* Player = nullptr;
	TheUFO* UFOs[2] = { nullptr };

	bool Separated = false;

	void ChasePlayer();
	void ChaseUFO();
	void LeaveScreen();
	void CheckCollisions();
	void Hit();
	void Destroy();
};

