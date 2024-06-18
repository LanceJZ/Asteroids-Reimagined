#include "Game.h"

Game::Game()
{
	LogicID = TheManagers.EM.AddCommon(Logic = DBG_NEW GameLogic());
	BackGroundID = TheManagers.EM.AddCommon(BackGround = DBG_NEW TheBackground());
	EnemiesID = TheManagers.EM.AddCommon(Enemies = DBG_NEW EnemyControl());
	PlayerID = TheManagers.EM.AddLineModel(Player = DBG_NEW ThePlayer());
}

Game::~Game()
{
}

bool Game::Initialize() //Initialize
{
	Common::Initialize(TheUtilities);

	float multi = 1.0f;
	FieldSize = { GetScreenWidth() * multi, (float)GetScreenHeight() };

	//When adding classes to EM, must be pointer to heap,IE: Name = new Class().

	Logic->SetPlayer(Player);
	Logic->SetEnemies(Enemies);

	Enemies->SetPlayer(Player);

	//Any Entities added after this point need this method fired manually.
	TheManagers.Initialize();
	Player->Initialize();
	Logic->Initialize();

	return true;
}

bool Game::Load()
{
	size_t shotModelID = TheManagers.CM.LoadTheLineModel("Dot");
	Player->SetModel(TheManagers.CM.LoadAndGetLineModel("PlayerShip"));
	Player->SetTurretModel(TheManagers.CM.LoadAndGetLineModel("PlayerTurret"));
	Player->SetShotModel(TheManagers.CM.GetLineModel(shotModelID));

	return true;
}

bool Game::BeginRun()
{
	//Any Entities added after this point need this method fired manually.
	TheManagers.BeginRun();

	Player->NewGame();

	return true;
}

void Game::ProcessInput()
{
	GameInput();
	TheManagers.EM.Input();
}


void Game::Update(float deltaTime)
{
	if (Paused)	return;

	TheManagers.EM.Update(deltaTime);
}

void Game::Draw3D()
{
	BeginMode3D(TheCamera);
	//3D Drawing here.

	TheManagers.EM.Draw3D();

#ifdef _DEBUG
	int fsx = int(FieldSize.x * 0.5f);
	int fsy = int(FieldSize.y * 0.5f);

	DrawLine(-fsx, -fsy, fsx, -fsy, { DARKBLUE });
	DrawLine(fsx, -fsy, fsx, fsy, { DARKBLUE }); //Right side.
	DrawLine(fsx, fsy - 1, -fsx, fsy, { DARKBLUE });
	DrawLine(-fsx + 1, fsy - 1, -fsx + 1, -fsy, { DARKBLUE }); //Left side.
#endif

	EndMode3D();
}

void Game::Draw2D()
{
	//2D drawing, fonts go here.
}

void Game::GameInput()
{
	if (IsKeyPressed(KEY_P))
	{
		Paused = !Paused;
	}

	Logic->GameInput();
}