#pragma once
#include "Globals.h"
#include "SharedMethods.h"
#include "ThePlayer.h"
#include "ThePlayerBase.h"

class ThePlayerDrone : public ThePlayerBase
{
public:
	ThePlayerDrone();
	virtual ~ThePlayerDrone();

	SharedMethods SM = SharedMethods();

	std::vector<Enemy*> Enemies = std::vector<Enemy*>();
	TheCurrentState CurrentState = TheCurrentState::Idle;

	void SetPlayer(ThePlayer* player);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void PlayerHit();
	void Spawn(Vector3 position);
	void Destroy();

private:
	bool PlayerWasHit = false;

	size_t MoveTimerID = 0;
	size_t FindPlaceToMoveTimerID = 0;
	size_t IdleTimerID = 0;

	Vector3 Waypoint = Vector3(0, 0, 0);

	ThePlayer *Player = nullptr;
	LineModel *Flame = nullptr;

	Entity *RockDetector = nullptr;

	void FireShot();
	void FindPlaceToMove();
	void Move();
	void CheckForRocks();
	void LeavePlayfield();
};
