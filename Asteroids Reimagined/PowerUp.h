#pragma once
#include "Globals.h"
#include "ThePlayer.h"

class PowerUp : public LineModel
{
public:
	enum PowerUpType
	{
		Red,
		Blue,
		Purple
	};

	PowerUpType Type = PowerUpType::Red;

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
	size_t LifeTimerID = 0;

	Sound PickUpSound = {};

	ThePlayer* Player = nullptr;

	void CheckCollisions();
};
