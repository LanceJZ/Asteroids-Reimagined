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

TheCurrentState CurrentState = TheCurrentState::Idle;

	std::vector<TheStarShot*> StarShots;

	void SetStarShotSound(std::string sound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void SetPlayer(ThePlayer *player);
	void Spawn(Vector3 position);
	void Destroy();

private:
	size_t MovingTimerID = 0;
	size_t ShootingTimerID = 0;
	size_t FireTiemrID = 0;
	Vector3 PlayerPosition = {};

	Sound StarShotSound = {};

	Entity* ShieldBorder = nullptr;
	Entity* RockBorder = nullptr;

	ThePlayer* Player = nullptr;

	void TurretTimers();

	void DoIdle();
	void DoShootingAtPlayer();
	void DoChasingPlayer();
	void DoShootingAtRock();
	void DoShootingAtEnemy();
	void DoMoveing();
};
