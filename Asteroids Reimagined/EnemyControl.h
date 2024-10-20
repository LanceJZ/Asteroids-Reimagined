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
	int Wave = 0;
	Vector3 PowerUpSpawnPosition = {};

	std::vector<TheRock*> Rocks;
	std::vector<TheUFO*> UFOs;
	std::vector<Enemy1*> EnemyOnes;
	std::vector<Enemy2*> EnemyTwos;

	TheDeathStar *DeathStar = nullptr;
	TheBoss *Boss = nullptr;

	LineModelPoints UFOModel = {};

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
	void SetLeftSpineMountModel(LineModelPoints model);
	void SetRightSpineMountModel(LineModelPoints model);
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
	void SetEnemyOneMissileOnSound(Sound sound);
	void SetEnemyOneExplodeSound(Sound sound);
	void SetEnemyOneOnSound(Sound sound);
	void SetEnemyOneMissileExplodeSound(Sound sound);
	void SetEnemyTwoSpawnSound(Sound sound);
	void SetEnemyTwoFireSound(Sound sound);
	void SetEnemyTwoExplodeSound(Sound sound);
	void SetEnemyTwoOnSound(Sound sound);
	void SetEnemyTwoMineExplodeSound(Sound sound);
	void SetBossExplodeSound(Sound sound);
	void SetBossShieldHitSound(Sound sound);
	void SetBossShieldDownSound(Sound sound);
	void SetBossHitSound(Sound sound);
	void SetBossTurretFireSound(Sound sound);
	void SetBossTurretExplodeSound(Sound sound);
	void SetBossSpineFireSound(Sound sound);

	void SetParticleManager(ParticleManager* particles);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();
	void FixedUpdate();

	void NewGame();
	void NextWave();

private:
	bool NoMoreRocks = false;
	bool SpawnedDeathStar = false;
	bool NewWave = false;
	bool BossWave = false;
	bool ClearForBossWave = false;

	size_t UFOSpawnTimerID;
	size_t DeathStarSpawnTimerID;
	size_t EnemyOneSpawnTimerID;
	size_t EnemyTwoSpawnTimerID;
	size_t BossExplodingTimerID;

	int UFOSpawnCount = 0;
	int StartRockCount = 4;
	int RockSpawnCount = 0;
	int RockCount = 0;
	int EnemyOneSpawnCount = 0;
	int EnemyTwoSpawnCount = 0;

	float EnemyOneSpawnTimeAmount = 0;
	float EnemyTwoSpawnTimeAmount = 0;
	float UFOSpawnTimeAmount = 0.0f;

	Sound RockExplodeSound = {};
	Sound UFOExplodeSound = {};
	Sound UFOFireSound = {};
	Sound UFOSmallSound = {};
	Sound UFOBigSound = {};
	Sound EnemyOneSpawnSound = {};
	Sound EnemyOneFireSound = {};
	Sound EnemyOneMissileOnSound = {};
	Sound EnemyOneExplodeSound = {};
	Sound EnemyOneOnSound = {};
	Sound EnemyOneMissileExplodeSound = {};
	Sound EnemyTwoSpawnSound = {};
	Sound EnemyTwoFireSound = {};
	Sound EnemyTwoExplodeSound = {};
	Sound EnemyTwoOnSound = {};
	Sound EnemyTwoMineExplodeSound = {};

	LineModelPoints ShotModel;
	LineModelPoints RockModels[4];
	LineModelPoints EnemyOneModel;
	LineModelPoints EnemyTwoModel;
	LineModelPoints MissileModel;
	LineModelPoints MineModel;

	ThePlayer *Player = nullptr;
	ParticleManager *Particles = nullptr;

	void SpawnRocks(Vector3 position, int count, TheRock::RockSize size);
	void SpawnUFO();
	void SpawnEnemyOne();
	void SpawnEnemyTwo();
	void SpawnDeathStar();
	void SpawnBoss();
	void CheckDeathStarStatus();
	void CheckRockCollisions();
	bool CheckUFOCollisions(TheRock* rock);
	bool CheckEnemyCollisions(TheRock* rock);
	void MakeReadyForBossWave();
	void DoBossStuff();
	void HaveHomingMineChaseEnemy();

	void Reset();
};