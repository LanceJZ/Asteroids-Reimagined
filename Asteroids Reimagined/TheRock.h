#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheAntiPlayer.h"
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

	void SetPlayer(ThePlayer* player);
	void SetAntiPlayer(TheAntiPlayer* player);
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

	ThePlayer *Player = nullptr;
	TheAntiPlayer *AntiPlayer = nullptr;

	bool CheckCollisions();
	void SendScoreToPlayer();
};