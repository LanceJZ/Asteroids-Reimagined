#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "ParticleManager.h"
#include "Enemy.h"

class TheMine : public Enemy
{
public:
	TheMine();
	virtual ~TheMine();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Hit();
	void Destroy();

protected:

private:
	size_t LifeTimerID = 0;

	bool CheckCollisions();
};
