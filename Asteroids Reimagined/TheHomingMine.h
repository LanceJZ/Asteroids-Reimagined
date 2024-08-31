#pragma once
#include "Globals.h"
#include "ParticleManager.h"

class TheHomingMine : public LineModel
{
public:
	TheHomingMine();
	virtual ~TheHomingMine();

	void SetExplodeSound(Sound sound);
	void SetParticleManager(ParticleManager* particles);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void ChaseEnemy(Vector3 enemyPosition);
	void LostEnemy();

	void Spawn(Vector3 position);
	void Destroy();

private:
	size_t LifeTimerID = 0;

	Sound ExplodeSound = {};

	ParticleManager* Particles = nullptr;

	void CheckIsEnemyClose();
	void CheckCollision();
};