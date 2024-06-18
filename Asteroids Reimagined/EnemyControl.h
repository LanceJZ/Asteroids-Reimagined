#pragma once
#include "Globals.h"
#include "ThePlayer.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	void SetPlayer(ThePlayer* player);

	bool Initialize();
	bool BeginRun();

	void Update();

private:
	ThePlayer *Player = nullptr;

	void Reset();

};

