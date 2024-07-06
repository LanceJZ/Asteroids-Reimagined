#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "ParticleManager.h"

class TheMine : public LineModel
{
public:
	TheMine();
	virtual ~TheMine();

	void SetPlayer(ThePlayer* player);
	void SetExplodeSound(Sound sound);

	void SetParticleManager(ParticleManager* particles);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Hit();
	void Destroy();

private:

	size_t LifeTimerID = 0;

	Sound ExplodeSound = {};

	ThePlayer* Player = nullptr;
	ParticleManager* Particles = nullptr;

	void CheckCollisions();
};
