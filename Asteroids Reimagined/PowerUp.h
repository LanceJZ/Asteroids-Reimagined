#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "EnemyControl.h"

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
		Maroon
	};

	PowerUpType Type = PowerUpType::Skyblue;

	int Wave = 0;

	PowerUp();
	virtual ~PowerUp();

	void SetPlayer(ThePlayer* player);
	void SetEnemy(EnemyControl* enemy);

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
	EnemyControl* Enemies = nullptr;

	void CheckCollisions();
	void DriftToPlayer();
};
