#pragma once
#include "Globals.h"
#include "Enemy.h"

class TheBossTurret : public Enemy
{
public:
	TheBossTurret();
	virtual ~TheBossTurret();

	std::vector<Shot*> Shots;

	//void SetPlayer(ThePlayer* player);

	//void SetShotModel(std::vector<Vector3> model);

	void SetFireSound(Sound sound);
	void SetExplodeSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	bool CheckCollisions();

	void Spawn();
	void Hit();
	void Destroy();
	void Reset();

private:
	//size_t ShotTimerID;

	float ShotTimerSetting = 0;

	//Sound FireSound = {};
	//Sound ExplodeSound = {};

	//std::vector<Vector3> ShotModel = {};

	//ThePlayer* Player = nullptr;

	void Fire();
};
