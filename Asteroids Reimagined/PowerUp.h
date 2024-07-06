#pragma once
#include "Globals.h"
#include "ThePlayer.h"

class PowerUp : public LineModel
{
public:
	PowerUp();
	virtual ~PowerUp();

	void SetPlayer(ThePlayer* player);

	void SetPickUpSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:
	bool Shield = false;
	bool Heat = false;

	size_t LifeTimerID = 0;

	Sound PickUpSound = {};

	ThePlayer* Player = nullptr;

	void CheckCollisions();
};
