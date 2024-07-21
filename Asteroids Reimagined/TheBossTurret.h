#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "Shot.h"

class TheBossTurret : public LineModel
{
public:
	TheBossTurret();
	virtual ~TheBossTurret();

	void SetPlayer(ThePlayer* player);
	void SetShotModel(LineModelPoints model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn();
	void Destroy();

private:
	size_t FireTimerID;

	float FireTimerSetting = 0;

	LineModelPoints ShotModel = {};
	ThePlayer* Player = nullptr;

	std::vector<Shot*> Shots;

	void Fire();
};
