#pragma once
#include "Globals.h"
#include "Enemy.h"

class TheStarShot : public Enemy
{
public:
	TheStarShot();
	virtual ~TheStarShot();

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Hit();
	void Destroy();

private:
	size_t LifeTimerID = 0;

	bool CheckCollisions();

};
