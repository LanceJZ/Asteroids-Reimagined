#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "Shot.h"

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

	Shot* Shots[2] = { nullptr };

	TheUFO();
	virtual ~TheUFO();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void SetPlayer(ThePlayer* player);
	void SetShotModel(LineModelPoints model);

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(int SpawnCount);
	void Destroy();

private:
	size_t FireTimerID = 0;
	size_t ChangeVectorTimerID = 0;

	LineModelPoints ShotModel;

	ThePlayer* Player = {};

	void FireShot();
	float AimedShot();
	float AimedShotAtDeathStar();
	float AimedShotAtRock();
	void ChangeVector();
	bool CheckReachedSide();
};