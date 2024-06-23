#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheUFO.h"
#include "TheFighterPair.h"

class TheDeathStar : public Entity
{
public:
	TheDeathStar();
	virtual ~TheDeathStar();

	TheFighterPair* FighterPairs[3] = { nullptr };

	void SetPlayer(ThePlayer* player);
	void SetUFO(TheUFO* ufo[2]);
	void SetWedgeModel(LineModelPoints model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);

private:
	int Score = 50;

	float WX = 0.0f;
	float WY = 0.0f;

	ThePlayer* Player = nullptr;
	TheUFO* UFOs[2] = {nullptr};

	void CheckCollisions();
	void Hit();
	void Destroy();
};

