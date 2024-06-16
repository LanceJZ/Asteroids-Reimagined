#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "Shot.h"
#include "ParticleManager.h"

class Enemy : public Model3D
{
public:
	Enemy();
	virtual ~Enemy();

	std::vector<Shot*> Shots;

	void SetPlayer(ThePlayer* player);
	void SetShotModel(Model model);
	void SetFireSound(Sound fireSound);
	void SetExplodeSound(Sound explodeSound);

	void SetParticleManager(ParticleManager* particleManager);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw();

	void Spawn(Vector3 position);
	void Destroy();
	void Hit();
	void Reset();

protected:
	size_t ShotTimerID = 0;

	int Points = 0;

	Sound FireSound = {};
	Sound ExplodeSound = {};

	ThePlayer* Player = nullptr;
	ParticleManager* PM = nullptr;

	void Shoot();
	void Shoot(Vector3 velocity);
	virtual bool CheckCollision();

private:

	Model ShotModel = { 0 };

};
