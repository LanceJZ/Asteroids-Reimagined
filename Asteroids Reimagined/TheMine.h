#pragma once
#include "Globals.h"
#include "ThePlayer.h"

class TheMine : public LineModel
{
public:
	TheMine();
	virtual ~TheMine();

	void SetPlayer(ThePlayer* player);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:

	size_t LifeTimerID = 0;

	ThePlayer* Player = nullptr;

	void CheckCollisions();
};
