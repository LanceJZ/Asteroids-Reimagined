#pragma once
#include "Globals.h"
#include "Enemy.h"
#include "ThePlayer.h"
#include "Shot.h"
//#include "TheRock.h"
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

	Vector3 DeathStarPosition = { 0.0f, 0.0f, 0.0f };

	//Shot* Shots[2] = { nullptr };
	std::vector<Entity*> Rocks;

	TheUFO();
	virtual ~TheUFO();

	//void SetRocks(std::vector<Entity*> &rocks);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	//void SetPlayer(ThePlayer* player);
	//void SetShotModel(LineModelPoints model);

	//void SetExplodeSound(Sound sound);
	//void SetFireSound(Sound sound);
	void SetBigSound(Sound sound);
	void SetSmallSound(Sound sound);
	//void SetParticles(ParticleManager* particles);

	void Update(float deltaTime);
	void Draw3D();

	bool CheckShotCollisions(Entity* rock);
	//void CheckShotPlayerCollisions();

	void Spawn(int SpawnCount);
	void Destroy();
	void Hit();
	void Reset();

	void CheckShotsHitPlayer();

private:
	size_t FireTimerID = 0;
	size_t ChangeVectorTimerID = 0;

	//Sound ExplodeSound;
	//Sound FireSound;
	Sound BigSound = {};
	Sound SmallSound = {};

	//LineModelPoints ShotModel;

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