#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheUFO.h"
#include "Shot.h"
#include "ParticleManager.h"

class Enemy : public LineModel
{
public:
	Enemy();
	virtual ~Enemy();

	std::vector<Shot*> Shots;

	void SetPlayer(ThePlayer* player);
	void SetUFO(TheUFO* ufos[2]);
	void SetShotModel(LineModelPoints model);
	void SetFireSound(Sound fireSound);
	void SetExplodeSound(Sound explodeSound);

	void SetParticleManager(ParticleManager* particleManager);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

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
	TheUFO* UFOs[2] = { nullptr };
	ParticleManager* Particles = nullptr;

	void Shoot();
	void Shoot(Vector3 velocity);
	void ChasePlayer();
	virtual bool CheckCollisions();

private:
	float Speed = 200.0f;
	float TurnSpeed = 1.45f;

	LineModelPoints ShotModel = {};


};