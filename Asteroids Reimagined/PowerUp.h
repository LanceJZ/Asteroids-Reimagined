#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheAntiPlayer.h"

class PowerUp : public LineModel
{
public:
	enum PowerUpType
	{
		Skyblue,
		Blue,
		Purple,
		Red,
		Yellow,
		Green,
		Orange,
		Violet,
		Magenta,
		Maroon,
		None
	};

	PowerUpType Type = PowerUpType::None;

	PowerUp();
	virtual ~PowerUp();

	void SetPlayer(ThePlayer* player);
	void SetAntiPlayer(TheAntiPlayer* player);
	void SetPickUpSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position, Vector3 velocity, PowerUpType type, int wave);
	void Destroy();

private:
	size_t LifeTimerID = 0;

	Sound PickUpSound = {};

	ThePlayer* Player = nullptr;
	TheAntiPlayer* AntiPlayer = nullptr;

	void CheckCollisions();
	void DriftToPlayer();
};
