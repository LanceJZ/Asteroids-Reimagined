#pragma once
#include "Globals.h"
#include "Enemy.h"

class TheMissile : public Enemy
{
public:
	TheMissile();
	virtual ~TheMissile();

	void SetOnSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Hit();
	void Destroy();

private:
	size_t LifeTimerID = 0;

	Sound OnSound = {};

	bool CheckCollisions();
};
