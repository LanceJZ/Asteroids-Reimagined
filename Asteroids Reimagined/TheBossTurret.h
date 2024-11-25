#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "Shot.h"

class TheBossTurret : public LineModel
{
public:
	TheBossTurret();
	virtual ~TheBossTurret();

	std::vector<Shot*> Shots;

	void SetPlayer(ThePlayer* player);

	void SetShotModel(LineModelPoints model);

	void SetFireSound(Sound sound);
	void SetExplodeSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void CheckCollisions();

	void Spawn();
	void Hit();
	void Destroy();
	void Reset();

private:
	size_t FireTimerID;

	float FireTimerSetting = 0;

	Sound FireSound = {};
	Sound ExplodeSound = {};

	LineModelPoints ShotModel = {};

	ThePlayer* Player = nullptr;

	void Fire();
};
