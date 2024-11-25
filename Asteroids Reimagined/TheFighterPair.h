#pragma once
#include "Globals.h"
#include "TheFighter.h"

class TheFighterPair : public Enemy
{
public:
	TheFighterPair();
	virtual ~TheFighterPair();

	bool NewWave = false;
	int Wave = 0;

	TheFighter* Fighters[2] = { nullptr };

	void SetPlayer(ThePlayer* player);
	void SetUFO(std::vector<Enemy*> ufos);
	void SetWedgeModel(LineModelPoints model);

	void SetExplodeSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Separate();
	void Destroy();
	void Reset();

	void Spawn(Vector3 position);

private:

	bool Separated = false;

	bool CheckCollisions();
	void Hit();
};

