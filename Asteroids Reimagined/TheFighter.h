#pragma once
#include "Globals.h"
#include "Enemy.h"
//#include "ThePlayer.h"
//#include "TheUFO.h"

class TheFighter : public Enemy
{
public:
	TheFighter();
	virtual ~TheFighter();

	bool NewWave = false;

	//void SetPlayer(ThePlayer* player);
	void SetEnemies(Enemy* enemyOne, Enemy* enemyTwo);
	void SetExplodeSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Separate();
	void Reset();
	void Destroy();

	void Spawn(Vector3 position);

	void ChasePlayer();
	void ChaseUFO();
	void ChaseEnemyOne();
	void ChaseEnemyTwo();
	void LeaveScreen();
	bool CheckCollisions();

private:
	bool Separated = false;
	//int Points = 200;
	//float Speed = 150.0f;
	//float TurnSpeed = 0.45f;
	//float RotateMagnitude = PI / 2;

	//Sound ExplodeSound;

	//ThePlayer* Player = nullptr;

	void Hit();
};