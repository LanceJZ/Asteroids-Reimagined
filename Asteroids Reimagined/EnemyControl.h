#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheRock.h"
#include "TheUFO.h"
#include "TheDeathStar.h"
#include "Enemy1.h"
#include "Enemy2.h"

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
	void SetWedgeModel(LineModelPoints model);
	void SetEnemy1Model(LineModelPoints model);
	void SetEnemy2Model(LineModelPoints model);
	void SetEnemyMissileModel(LineModelPoints model);
	void SetEnemyMineModel(LineModelPoints model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

private:
	bool NoMoreRocks = false;
	bool SpawnedDeathStar = false;
	bool NewWave = false;

	size_t UFOSpawnTimerID;
	size_t DeathStarSpawnTimerID;
	size_t EnemyOneSpawnTimerID;
	size_t EnemyTwoSpawnTimerID;

	int UFOSpawnCount = 0;
	int StarRockCount = 4;
	int RockSpawnCount = 0;
	int RockCount = 0;
	int Wave = 0;

	LineModelPoints ShotModel;
	LineModelPoints RockModels[4];

	ThePlayer *Player = nullptr;
	TheDeathStar *DeathStar = nullptr;
	Enemy1 *EnemyOne = nullptr;
	Enemy2 *EnemyTwo = nullptr;

	void SpawnRocks(Vector3 position, int count, TheRock::RockSize size);
	void SpawnUFO();
	void SpawnDeathStar();
	void CheckDeathStarStatus();
	bool CheckRockCollisions();
	bool CheckUFOCollisions(TheRock* rock);
	bool CheckEnemyCollisions(TheRock* rock);

	void Reset();
};