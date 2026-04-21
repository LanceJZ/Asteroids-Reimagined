#pragma once
#include "Globals.h"
#include "ThePlayerControls.h"
#include "TheStarShot.h"

enum TheCurrentState
{
	Idle,
	ShootingAtPlayer,
	ChasingPlayer,
	ShootingAtRock,
	ShootingAtEnemy,
	Moveing
};

class TheAntiPlayer : public ThePlayerControls
{
public:
	TheAntiPlayer();
	virtual ~TheAntiPlayer();

	bool NearbyRockOrEnemy = false;
	bool SafeToSpawn = false;
	bool CheckSafeToSpawn = false;

	Vector3 RockOrEnemyPosition = {};

	TheCurrentState CurrentState = TheCurrentState::Idle;

	Entity* ShieldBorder = nullptr;
	Entity* AvoidBorder = nullptr;

	std::vector<TheStarShot*> StarShots;

	void SetStarShotSound(std::string sound);
	void SetPlayer(ThePlayer *player);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void AttackRocksOrEnemies(Vector3 position);
	void ActivateTheShield();
	void DeactivateTheShield();
	void Spawn(Vector3 position);
	void Destroy();

private:
	bool AvoidRocksOrEnemies = false; //avoid rocks or enemies.
	bool ActivateShield = false; //activate shield

	size_t MovingTimerID = 0;
	size_t ShootingTimerID = 0;
	size_t FireTimerID = 0;
	size_t IdelTimerID = 0;
	size_t AttackRockTimerID = 0;

	Vector3 PlayerPosition = {};

	Sound StarShotSound = {};

	ThePlayer* Player = nullptr;

	void TurretTimers();
	void FireTurret();
	void DoIdle();
	void DoShooting();
	void DoChasingPlayer();
	void DoShootingAtRock();
	void DoShootingAtEnemy();
	void DoMoveing();
};
