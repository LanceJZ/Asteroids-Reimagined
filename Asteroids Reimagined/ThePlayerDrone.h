#pragma once
#include "Globals.h"
#include "SharedMethods.h"
#include "ThePlayer.h"
#include "ThePlayerBase.h"

class ThePlayerDrone : public ThePlayerBase
{
public:
	ThePlayerDrone();
	virtual ~ThePlayerDrone();

	SharedMethods SM = SharedMethods();

	void SetPlayer(ThePlayer* player);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:
	ThePlayer *Player = nullptr;
	LineModel *Flame = nullptr;

	Entity *RockDetector = nullptr;

	void FireShot();


};
