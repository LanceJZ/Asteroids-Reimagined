#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheAntiPlayer.h"
#include "Enemy.h"
#include "PowerUp.h"

class TheRock : public LineModel
{
public:
	enum RockSize
	{
		Small,
		Medium,
		MediumLarge,
		Large
	};

	TheRock();
	virtual ~TheRock();

	int WaveNumber = 0;

	RockSize Size = Large;
	PowerUp::PowerUpType PowerUpType = PowerUp::PowerUpType::None;

	void SetPlayer(ThePlayer* player);
	void SetAntiPlayer(TheAntiPlayer* player);
	void SetPowerUpModel(std::vector<Vector3> model);
	void SetExplodeSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position, RockSize size = Large);
	void Destroy();
	void Hit();

private:
	Sound ExplodeSound = {};

	LineModel* PowerUp = nullptr;

	ThePlayer *Player = nullptr;
	TheAntiPlayer *AntiPlayer = nullptr;

	bool CheckCollisions();
	void SendScoreToPlayer();
};