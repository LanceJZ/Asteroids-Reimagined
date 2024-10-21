#pragma once
#include <memory>
#include "Globals.h"
#include "GameLogic.h"
#include "ThePlayer.h"
#include "EnemyControl.h"
#include "TheBackground.h"
#include "Particlemanager.h"

class Game : Common
{
public:
	Game();
	virtual ~Game();

	GameLogic* Logic = {};

	bool Initialize(Utilities* utilities);
	bool Load();
	bool BeginRun();
	void ProcessInput();
	void Update(double deltaTime);
	void FixedUpdate(double deltaTime);
	void Draw3D();
	void Draw2D();

private:
	void GameInput();

	bool CursorDisabled = true;
	size_t LogicID = 0;
	size_t BackGroundID = 0;
	size_t PlayerID = 0;
	size_t EnemiesID = 0;
	size_t ParticlesID = 0;

	//When adding classes to EM, must be heap pointer.
	//TheBackground* BackGround = {};
	ThePlayer* Player = {};
	EnemyControl* Enemies = {};
	ParticleManager* Particles = {};
};

