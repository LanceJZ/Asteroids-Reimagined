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

	DisableCursor();

	float multi = 1.0f;
	FieldSize = { GetScreenWidth() * multi, (float)GetScreenHeight() };

	//When adding classes to EM, must be pointer to heap,IE: Name = new Class().

	Logic->SetPlayer(Player);
	Logic->SetEnemies(Enemies);

	Enemies->SetPlayer(Player);

	//Any Entities added after this point need this method fired manually.
	TheManagers.Initialize();

	return true;
}

bool Game::Load()
{
	size_t shotModelID = TheManagers.CM.LoadTheLineModel("Dot");
	Player->SetModel(TheManagers.CM.LoadAndGetLineModel("PlayerShip"));
	Player->SetFlameModel(TheManagers.CM.LoadAndGetLineModel("PlayerFlame"));
	Player->SetShieldModel(TheManagers.CM.LoadAndGetLineModel("PlayerShield"));
	Player->SetTurretModel(TheManagers.CM.LoadAndGetLineModel("PlayerTurret"));
	Player->SetShotModel(TheManagers.CM.GetLineModel(shotModelID));
	Player->SetCrosshairModel(TheManagers.CM.LoadAndGetLineModel("Cross"));

	Enemies->SetUFOModel(TheManagers.CM.LoadAndGetLineModel("UFO"));
	Enemies->SetShotModel(TheManagers.CM.GetLineModel(shotModelID));
	Enemies->SetWedgeModel(TheManagers.CM.LoadAndGetLineModel("Wedge"));
	Enemies->SetEnemy1Model(TheManagers.CM.LoadAndGetLineModel("Enemy1"));
	Enemies->SetEnemy2Model(TheManagers.CM.LoadAndGetLineModel("Enemy2"));
	Enemies->SetEnemyMissileModel(TheManagers.CM.LoadAndGetLineModel("Missile"));
	Enemies->SetEnemyMineModel(TheManagers.CM.LoadAndGetLineModel("Mine"));

	LineModelPoints rockModels[4];

	rockModels[0] = TheManagers.CM.LoadAndGetLineModel("Rock1");
	rockModels[1] = TheManagers.CM.LoadAndGetLineModel("Rock2");
	rockModels[2] = TheManagers.CM.LoadAndGetLineModel("Rock3");
	rockModels[3] = TheManagers.CM.LoadAndGetLineModel("Rock4");

	Enemies->SetRockModels(rockModels);

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
	if (IsGamepadAvailable(0) && CursorDisabled)
	{
		EnableCursor();
		CursorDisabled = false;
	}
	else if (!CursorDisabled && !IsGamepadAvailable(0))
	{
		DisableCursor();
		CursorDisabled = true;
	}

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

	DrawLine(-fsx, -fsy, fsx, -fsy, { DARKBLUE }); //Top.
	DrawLine(fsx, -fsy, fsx, fsy, { DARKBLUE }); //Right side.
	DrawLine(fsx, fsy - 1, -fsx, fsy - 1, { DARKBLUE }); //Bottom.
	DrawLine(-fsx + 1, fsy - 1, -fsx + 1, -fsy - 1, { DARKBLUE }); //Left side.
#endif

	EndMode3D();
}

void Game::Draw2D()
{
	//2D drawing, fonts go here.
	TheManagers.EM.Draw2D();
}

void Game::GameInput()
{
	if (IsKeyPressed(KEY_P))
	{
		Paused = !Paused;
	}

	Logic->GameInput();
}