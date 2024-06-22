#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheFighterPair.h"

class TheDeathStar : public Entity
{
public:
	TheDeathStar();
	virtual ~TheDeathStar();

	TheFighterPair* FighterPairs[3] = { nullptr };

	void SetWedgeModel(LineModelPoints model);
	void SetPlayer(ThePlayer* player);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);

private:
	float WX = 0.0f;
	float WY = 0.0f;

	ThePlayer* Player = nullptr;

	bool CheckCollisions();
	void Hit();
	void Destroy();
};

