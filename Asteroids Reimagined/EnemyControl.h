#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheRock.h"
#include "TheUFO.h"
#include "TheDeathStar.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "TheBoss.h"
#include "TheAntiPlayer.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	bool SpawnPowerUp = false;

	int WaveNumber = 0;

	PowerUp::PowerUpType PowerUpType = PowerUp::PowerUpType::None;

	Vector3 PowerUpSpawnPosition = {};
	Vector3 PowerUpSpawnVelocity = {};

	std::vector<TheRock*> Rocks;
	std::vector<TheUFO*> UFOs;
	std::vector<Enemy1*> EnemyOnes;
	std::vector<Enemy2*> EnemyTwos;

	TheDeathStar *DeathStar = nullptr;
	TheBoss *Boss = nullptr;
	TheAntiPlayer *AntiPlayer = nullptr;

	std::vector<Vector3> UFOModel = {};
	//Models
	void SetPlayer(ThePlayer* player);
	void SetAntiPlayer(TheAntiPlayer* player);
	void SetRockModels(std::vector<Vector3> rockModels[4]);
	void SetUFOModel(std::vector<Vector3> model);
	void SetShotModel(std::vector<Vector3> model);
	void SetWedgeModel(std::vector<Vector3> model);
	void SetEnemy1Model(std::vector<Vector3> model);
	void SetEnemy2Model(std::vector<Vector3> model);
	void SetBossModel(std::vector<Vector3> model);
	void SetBossShieldModel(std::vector<Vector3> model);
	void SetBossTurretModel(std::vector<Vector3> model);
	void SetLeftSpineMountModel(std::vector<Vector3> model);
	void SetRightSpineMountModel(std::vector<Vector3> model);
	void SetEnemyMissileModel(std::vector<Vector3> model);
	void SetEnemyMineModel(std::vector<Vector3> model);
	void SetAntiPlayerModel(std::vector<Vector3> model);
	void SetPowerUpModel(std::vector<Vector3> model);
	// Sounds
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
	void SetBossOnSound(Sound sound);
	void SetBossExplodeSound(Sound sound);
	void SetBossShieldHitSound(Sound sound);
	void SetBossShieldDownSound(Sound sound);
	void SetBossHitSound(Sound sound);
	void SetBossTurretFireSound(Sound sound);
	void SetBossTurretExplodeSound(Sound sound);
	void SetBossSpineFireSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Update();
	void FixedUpdate();

	void NextWave();
	void NewGame();
	void Reset();

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
	size_t SpawnAntiPlayerTimerID;

	int UFOSpawnCount = 0;
	int StartRockCount = 4;
	int RockSpawnCount = 0;
	int RockCount = 0;
	int EnemyOneSpawnCount = 0;
	int EnemyTwoSpawnCount = 0;

	float const EnemyOneSpawnTimeAmount = 15.0f;
	float EnemyTwoSpawnTimeAmount = 20.0f;
	float const UFOSpawnTimeAmount = 30.0f;

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

	std::vector<Vector3> ShotModel;
	std::vector<Vector3> RockModels[4];
	std::vector<Vector3> EnemyOneModel;
	std::vector<Vector3> EnemyTwoModel;
	std::vector<Vector3> MissileModel;
	std::vector<Vector3> MineModel;
	std::vector<Vector3> PowerUpModel;

	ThePlayer *Player = nullptr;

	void SpawnRocks(Vector3 position, int count, TheRock::RockSize size);
	void SpawnUFO();
	void SpawnEnemyOne();
	void SpawnEnemyTwo();
	void SpawnDeathStar();
	void SpawnBoss();
	void SpawnAntiPlayer();
	void CheckDeathStarStatus();
	void CheckRockCollisions();
	bool CheckUFORockCollisions(TheRock* rock);
	bool CheckEnemyRockCollisions(TheRock* rock);
	bool CheckAntiPlayerRockCollisions(TheRock* rock);
	void MakeReadyForBossWave();
	void DoBossStuff();
	void HaveHomingMineChaseEnemy();
};