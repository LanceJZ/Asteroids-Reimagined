#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "EnemyControl.h"

class PowerUp : public LineModel
{
public:
	enum PowerUpType
	{
		Red,
		Blue,
		Purple,
		Green,
		Yellow,
		White,
		Orange,
		Violet,
		Magenta
	};

	PowerUpType Type = PowerUpType::Red;

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
