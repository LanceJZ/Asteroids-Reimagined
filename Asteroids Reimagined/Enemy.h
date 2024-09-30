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

	int Wave = 0;
	float Distance = 0.0f;
	float ShotTimerAmount = 0.0f;

	std::vector<Shot*> Shots;
	std::vector<Enemy*> UFORefs;

	void SetPlayer(ThePlayer* player);
	void SetParticleManager(ParticleManager* particleManager);

	void SetShotModel(LineModelPoints model);

	void SetFireSound(Sound fireSound);
	void SetExplodeSound(Sound explodeSound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	virtual void Destroy();
	virtual void Hit();
	virtual void Reset();

protected:
	size_t ShotTimerID = 0;

	int Points = 0;
	float Speed = 200.0f;
	float TurnSpeed = 1.45f;
	float RotateMagnitude = PI / 2;

	ThePlayer* Player = nullptr;
	ParticleManager* Particles = nullptr;
	Sound FireSound = {};
	Sound ExplodeSound = {};
	Sound OnSound = {};
	Sound SpawnSound = {};
	LineModelPoints ShotModel = {};
	Enemy* EnemyOne = nullptr;
	Enemy* EnemyTwo = nullptr;

	void Shoot();
	void Shoot(Vector3 velocity);
	void ChasePlayer();
	void ChaseUFO();
	void ChaseEnemy();
	bool CheckUFOActive();
	bool LeaveScreen();
	virtual bool CheckCollisions();

private:
	void ChaseEnemyOne();
	void ChaseEnemyTwo();
};