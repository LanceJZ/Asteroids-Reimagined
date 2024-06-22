#pragma once
#include "Globals.h"
#include "TheFighter.h"

class TheFighterPair : public Entity
{
public:
	TheFighterPair();
	virtual ~TheFighterPair();

	TheFighter* Fighters[2] = { nullptr };

	void SetWedgeModel(LineModelPoints model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);

private:


};

