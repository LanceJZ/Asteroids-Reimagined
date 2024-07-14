#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheRock.h"
#include "TheUFO.h"
#include "TheDeathStar.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "TheBoss.h"
#include "ParticleManager.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	bool SpawnPowerUp = false;
	Vector3 PowerUpSpawnPosition = {};

	std::vector<TheRock*> Rocks;
	TheUFO* UFOs[2] = { nullptr };
	TheDeathStar *DeathStar = nullptr;
	Enemy1 *EnemyOne = nullptr;
	Enemy2 *EnemyTwo = nullptr;
	TheBoss *Boss = nullptr;

	void SetPlayer(ThePlayer* player);
	void SetRockModels(LineModelPoints rockModels[4]);
	void SetUFOModel(LineModelPoints model);
	void SetShotModel(LineModelPoints model);
	void SetWedgeModel(LineModelPoints model);
	void SetEnemy1Model(LineModelPoints model);
	void SetEnemy2Model(LineModelPoints model);
	void SetBossModel(LineModelPoints model);
	void SetBossShieldModel(LineModelPoints model);
	void SetBossTurretModel(LineModelPoints model);
	void SetEnemyMissileModel(LineModelPoints model);
	void SetEnemyMineModel(LineModelPoints model);

	void SetRockExplodeSound(Sound sound);
	void SetUFOExplodeSound(Sound sound);
	void SetUFOFireSound(Sound sound);
	void SetUFOBigSound(Sound sound);
	void SetUFOSmallSound(Sound sound);
	void SetDeathStarSpawnSound(Sound sound);
	void SetDeathStarExplodeSound(Sound sound);
	void SetEnemyOneSpawnSound(Sound sound);
	void SetEnemyOneFireSound(Sound sound);
	void SetEnemyOneExplodeSound(Sound sound);
	void SetEnemyOneOnSound(Sound sound);
	void SetEnemyOneMissileExplodeSound(Sound sound);
	void SetEnemyTwoSpawnSound(Sound sound);
	void SetEnemyTwoFireSound(Sound sound);
	void SetEnemyTwoExplodeSound(Sound sound);
	void SetEnemyTwoOnSound(Sound sound);
	void SetEnemyTwoMineExplodeSound(Sound sound);

	void SetParticleManager(ParticleManager* particles);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

	void NewGame();
	void NextWave();

private:
	bool NoMoreRocks = false;
	bool SpawnedDeathStar = false;
	bool NewWave = false;

	size_t UFOSpawnTimerID;
	size_t DeathStarSpawnTimerID;
	size_t EnemyOneSpawnTimerID;
	size_t EnemyTwoSpawnTimerID;

	int UFOSpawnCount = 0;
	int StartRockCount = 4;
	int RockSpawnCount = 0;
	int RockCount = 0;
	int Wave = 0;

	Sound RockExplodeSound = {};
	Sound UFOExplodeSound = {};
	Sound UFOFireSound = {};
	Sound UFOSmallSound = {};
	Sound UFOBigSound = {};

	LineModelPoints ShotModel;
	LineModelPoints RockModels[4];

	ThePlayer *Player = nullptr;
	ParticleManager *Particles = nullptr;

	void SpawnRocks(Vector3 position, int count, TheRock::RockSize size);
	void SpawnUFO();
	void SpawnDeathStar();
	void CheckDeathStarStatus();
	void CheckRockCollisions();
	void CheckUFOCollisions(TheRock* rock);
	void CheckEnemyCollisions(TheRock* rock);

	void Reset();
};