#pragma once
#include "Globals.h"

class TheHomingMine : public LineModel
{
public:
	TheHomingMine();
	virtual ~TheHomingMine();

	bool GameOver = false;

	void SetExplodeSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void ChaseEnemy(Vector3 enemyPosition);
	void ChaseEnemy(Entity* enemy);
	void LostEnemy();

	void Spawn(Vector3 position);
	void Hit();
	void Destroy();

private:
	size_t LifeTimerID = 0;

	Sound ExplodeSound = {};

	Entity* Target = nullptr;
};
