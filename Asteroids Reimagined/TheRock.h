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

	void SetPlayer(ThePlayer* player);
	void SetExplodeSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(double deltaTime);
	void FixedUpdate(double deltaTime);
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