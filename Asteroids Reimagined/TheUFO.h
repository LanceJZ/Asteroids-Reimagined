#pragma once
#include "Globals.h"
#include "Enemy.h"
#include "ThePlayer.h"
#include "Shot.h"
#include "TheRock.h"
#include "ParticleManager.h"

class TheUFO : public Enemy
{
public:
	enum Size
	{
		Small,
		Large
	};

	Size TheSize = Large;

	bool DeathStarActive = false;
	int Wave = 0;
	float Distance = 0.0f;

	Vector3 DeathStarPosition = { 0.0f, 0.0f, 0.0f };

	Shot* Shots[2] = { nullptr };

	TheUFO();
	virtual ~TheUFO();

	void SetRocks(std::vector<TheRock*> &rocks);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void SetPlayer(ThePlayer* player);
	void SetShotModel(LineModelPoints model);

	void SetExplodeSound(Sound sound);
	void SetFireSound(Sound sound);
	void SetBigSound(Sound sound);
	void SetSmallSound(Sound sound);
	void SetParticles(ParticleManager* particles);

	void Update(float deltaTime);
	void Draw3D();

	void CheckCollisions(TheRock* rock);
	void CheckShotPlayerCollisions();

	void Spawn(int SpawnCount);
	void Destroy();
	void Hit();
	void Reset();

private:
	size_t FireTimerID = 0;
	size_t ChangeVectorTimerID = 0;

	Sound ExplodeSound;
	Sound FireSound;
	Sound BigSound;
	Sound SmallSound;

	LineModelPoints ShotModel;

	std::vector<TheRock*> *Rocks;

	void FireShot();
	float AimedShot();
	float AimedShotAtDeathStar();
	float AimedShotAtRock();
	void ChangeVector();
	bool CheckReachedSide();
	bool CheckCollisions();
	void SendScoreToPlayer();
	void PlayOnSound();
};