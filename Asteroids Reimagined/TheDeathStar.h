#pragma once
#include "Globals.h"
#include "TheFighterPair.h"

class TheDeathStar : public Enemy
{
public:
	TheDeathStar();
	virtual ~TheDeathStar();

	bool NewWave = false;
	int Wave = 0;

	TheFighterPair* FighterPairs[3] = { nullptr };

	void SetPlayer(ThePlayer* player);
	void SetUFO();
	void SetWedgeModel(LineModelPoints model);

	void SetExplodeSound(Sound sound);
	void SetSpawnSound(Sound sound);
	void SetParticleManager(ParticleManager* particleManager);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void NewWaveStart();
	void Reset();
	void Spawn(Vector3 position);
	void NewGame();

private:
	int Points = 50;

	float WX = 0.0f;
	float WY = 0.0f;

	Sound SpawnSound = {};

	void Hit();
	void Destroy();
	bool CheckCollisions();
};

