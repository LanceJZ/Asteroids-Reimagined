#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheRock.h"
#include "TheUFO.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	std::vector<TheRock*> Rocks;
	TheUFO* UFOs[2] = { nullptr };

	void SetPlayer(ThePlayer* player);
	void SetRockModels(LineModelPoints rockModels[4]);
	void SetUFOModel(LineModelPoints model);
	void SetShotModel(LineModelPoints model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

private:
	bool NoMoreRocks = false;

	size_t UFOSpawnTimerID;

	int UFOSpawnCount = 0;
	int StarRockCount = 4;
	int RockCount = 0;

	LineModelPoints ShotModel;
	LineModelPoints RockModels[4];

	ThePlayer *Player = nullptr;

	void SpawnRocks(Vector3 position, int count, TheRock::RockSize size);
	void SpawnUFO();
	bool CheckRockCollisions();
	bool CheckUFOCollisions(TheRock* rock);

	void Reset();
};