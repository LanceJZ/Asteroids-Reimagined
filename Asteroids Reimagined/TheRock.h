#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "Enemy.h"

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

	RockSize Size = Large;
	Enemy* UFOs[2] = { nullptr };

	void SetPlayer(ThePlayer* player);
	void SetUFO(Enemy* ufo[2]);
	void SetExplodeSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position, RockSize size = Large);
	void Destroy();
	void Hit();

private:
	Sound ExplodeSound = {};

	ThePlayer *Player = nullptr;

	bool CheckCollisions();
	void SendScoreToPlayer();
};