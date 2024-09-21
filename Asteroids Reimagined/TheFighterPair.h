#pragma once
#include "Globals.h"
#include "TheFighter.h"
#include "Enemy.h"
//#include "ThePlayer.h"
//#include "TheUFO.h"

class TheFighterPair : public Entity
{
public:
	TheFighterPair();
	virtual ~TheFighterPair();

	bool NewWave = false;

	TheFighter* Fighters[2] = { nullptr };

	void SetPlayer(ThePlayer* player);
	void SetUFO(Enemy* ufo[2]);
	void SetWedgeModel(LineModelPoints model);

	void SetExplodeSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Separate();
	void Destroy();
	void Reset();

	void Spawn(Vector3 position);

private:
	int Points = 200;
	float Speed = 100.0f;
	float TurnSpeed = 0.5f;
	float RotateMagnitude = PI / 2;

	Sound ExplodeSound;

	ThePlayer* Player = nullptr;
	Enemy* UFOs[2] = { nullptr };

	bool Separated = false;

	void ChasePlayer();
	void ChaseUFO();
	void LeaveScreen();
	void CheckCollisions();
	void Hit();
};

