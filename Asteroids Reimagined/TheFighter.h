#pragma once
#include "Globals.h"
#include "Enemy.h"

class TheFighter : public Enemy
{
public:
	TheFighter();
	virtual ~TheFighter();

	bool NewWave = false;
	int Wave = 0;

	void SetExplodeSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Separate();
	void Reset();
	void Destroy();

	void Spawn(Vector3 position);

	bool CheckCollisions();

private:
	bool Separated = false;

	void Hit();
};