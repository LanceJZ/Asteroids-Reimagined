#pragma once
#include <memory>
#include "Globals.h"
#include "Utilities.h"
#include "GameLogic.h"
#include "ThePlayer.h"
#include "EnemyControl.h"
#include "TheBackground.h"

class Game : Common
{
public:
	Game();
	virtual ~Game();

	bool Paused = false;

	bool Initialize(Utilities &utilities, GameLogic* gameLogic);
	bool Load();
	bool BeginRun();
	void ProcessInput();
	void Update(float deltaTime);
	void Draw();

private:
	void Draw3D();
	void Draw2D();
	void GameInput();

	size_t LogicID = 0;
	size_t BackGroundID = 0;
	size_t PlayerID = 0;
	size_t EnemiesID = 0;
	//When adding classes to EM, must be heap pointer.
	GameLogic* Logic = {};
	TheBackground* BackGround = {};
	ThePlayer* Player = {};
	EnemyControl* Enemies = {};
};

