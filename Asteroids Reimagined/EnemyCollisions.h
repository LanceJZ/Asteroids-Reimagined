#pragma once
#include "Globals.h"
#include "ThePlayer.h"

class EnemyCollisions : public Entity
{
public:
	EnemyCollisions();
	virtual ~EnemyCollisions();

	void SetPlayer(ThePlayer* player);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

private:
	ThePlayer* Player = nullptr;

	Entity* Enemy1 = nullptr;
	Entity* Enemy2 = nullptr;
	Entity* UFOs[2] = { nullptr };
};

