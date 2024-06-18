#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheRock.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	std::vector<TheRock*> Rocks;

	void SetPlayer(ThePlayer* player);
	void SetRockModels(LineModelPoints rockModels[4]);

	bool Initialize();
	bool BeginRun();

	void Update();

private:
	LineModelPoints RockModels[4];

	ThePlayer *Player = nullptr;

	void SpawnRocks(Vector3 position, int count, TheRock::RockSize size);

	void Reset();
};