#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "Shot.h"
#include "ParticleManager.h"

class Enemy : public LineModel
{
public:
	Enemy();
	virtual ~Enemy();

	float Distance = 0.0f;
	float ShotTimerAmount = 0.0f;

	std::vector<Shot*> Shots;
	Enemy* UFOs[2] = { nullptr };

	ThePlayer* Player = nullptr;
	ParticleManager* Particles = nullptr;

	Sound ExplodeSound = {};

	void SetPlayer(ThePlayer* player);
	void SetUFO(Enemy* ufo[2]);
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
	float Speed = 200.0f;
	float TurnSpeed = 1.45f;
	float RotateMagnitude = PI / 2;

	Sound FireSound = {};
	LineModelPoints ShotModel = {};
	Enemy* EnemyOne = nullptr;
	Enemy* EnemyTwo = nullptr;

	void Shoot();
	void Shoot(Vector3 velocity);
	void ChasePlayer();
	void ChaseUFO();
	virtual bool CheckCollisions();

private:
};