#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "Shot.h"
#include "TheRock.h"

class TheUFO : public LineModel
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

	Shot* Shots[2] = { nullptr };

	TheUFO();
	virtual ~TheUFO();

	void SetRocks(std::vector<TheRock*> &rocks);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void SetPlayer(ThePlayer* player);
	void SetShotModel(LineModelPoints model);

	void Update(float deltaTime);
	void Draw3D();

	void CheckCollisions(TheRock* rock);

	void Spawn(int SpawnCount);
	void Destroy();

private:
	size_t FireTimerID = 0;
	size_t ChangeVectorTimerID = 0;

	LineModelPoints ShotModel;

	ThePlayer* Player = {};
	std::vector<TheRock*> *Rocks;

	void FireShot();
	float AimedShot();
	float AimedShotAtDeathStar();
	float AimedShotAtRock();
	void ChangeVector();
	bool CheckReachedSide();
	bool CheckCollisions();
	void SendScoreToPlayer();
};