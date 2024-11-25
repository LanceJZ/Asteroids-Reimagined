#pragma once
#include "Globals.h"
#include "Enemy.h"
#include "ThePlayer.h"
#include "Shot.h"

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

	std::vector<Entity*> Rocks;

	TheUFO();
	virtual ~TheUFO();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void SetBigSound(Sound sound);
	void SetSmallSound(Sound sound);

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Spawn(int SpawnCount);
	void Destroy();
	void Hit();
	void Reset();

private:
	size_t FireTimerID = 0;
	size_t ChangeVectorTimerID = 0;

	Sound BigSound = {};
	Sound SmallSound = {};

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