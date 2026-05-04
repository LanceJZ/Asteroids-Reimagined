#pragma once
#include "Globals.h"
#include "ThePlayer.h"

class ThePlayerDrone : public LineModel
{
public:
	ThePlayerDrone();
	virtual ~ThePlayerDrone();

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
};
