#include "Game.h"

Game::Game()
{
	//When adding classes to EM, must be pointer to heap,IE: Name = new Class().

	LogicID = Managers.EM.AddCommon(Logic = DBG_NEW GameLogic());
	BackGroundID = Managers.EM.AddCommon(BackGround = DBG_NEW TheBackground());
	EnemiesID = Managers.EM.AddCommon(Enemies = DBG_NEW EnemyControl());
	PlayerID = Managers.EM.AddLineModel(Player = DBG_NEW ThePlayer());
	ParticlesID = Managers.EM.AddCommon(Particles = DBG_NEW ParticleManager());
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

	Logic->SetPlayer(Player);
	Logic->SetEnemies(Enemies);

	Enemies->SetPlayer(Player);

	//Any Entities added after this point need this method fired manually.
	Managers.Initialize();

	return true;
}

bool Game::Load()
{
	//Models
	size_t shotModelID = Managers.CM.LoadTheLineModel("Dot");
	Player->SetModel(Managers.CM.LoadAndGetLineModel("PlayerShip"));
	Player->SetFlameModel(Managers.CM.LoadAndGetLineModel("PlayerFlame"));
	Player->SetShieldModel(Managers.CM.LoadAndGetLineModel("PlayerShield"));
	Player->SetTurretModel(Managers.CM.LoadAndGetLineModel("PlayerTurret"));
	Player->SetShotModel(Managers.CM.GetLineModel(shotModelID));
	Player->SetCrosshairModel(Managers.CM.LoadAndGetLineModel("Cross"));
	Player->SetTurretHeatModel(Managers.CM.LoadAndGetLineModel("HeatMeter"));
	Player->SetBigShotModel(Managers.CM.LoadAndGetLineModel("PlayerBigShot"));
	Player->SetMineModel(Managers.CM.LoadAndGetLineModel("HomingMine"));

	Enemies->SetUFOModel(Managers.CM.LoadAndGetLineModel("UFO"));
	Enemies->SetShotModel(Managers.CM.GetLineModel(shotModelID));
	Enemies->SetWedgeModel(Managers.CM.LoadAndGetLineModel("Wedge"));
	Enemies->SetEnemy1Model(Managers.CM.LoadAndGetLineModel("Enemy1"));
	Enemies->SetEnemy2Model(Managers.CM.LoadAndGetLineModel("Enemy2"));
	Enemies->SetEnemyMissileModel(Managers.CM.LoadAndGetLineModel("Missile"));
	Enemies->SetEnemyMineModel(Managers.CM.LoadAndGetLineModel("Mine"));
	Enemies->SetEnemyTwoMineExplodeSound(Managers.CM.LoadAndGetSound("MineExplode"));
	Enemies->SetBossModel(Managers.CM.LoadAndGetLineModel("BossShip"));
	Enemies->SetBossShieldModel(Managers.CM.LoadAndGetLineModel("BossShield"));
	Enemies->SetBossTurretModel(Managers.CM.LoadAndGetLineModel("BossTurret"));
	Enemies->SetLeftSpineMountModel(Managers.CM.LoadAndGetLineModel("LeftSpineGun"));
	Enemies->SetRightSpineMountModel(Managers.CM.LoadAndGetLineModel("RightSpineGun"));

	Logic->SetPowerUpModel(Managers.CM.LoadAndGetLineModel("PowerUp"));

	//Sounds
	Player->SetExplodeSound(Managers.CM.LoadAndGetSound("PlayerExplode"));
	Player->SetBonusSound(Managers.CM.LoadAndGetSound("BonusShip"));
	Player->SetFireSound(Managers.CM.LoadAndGetSound("PlayerFire"));
	Player->SetShieldHitSound(Managers.CM.LoadAndGetSound("PlayerShieldHit"));
	Player->SetShieldOnSound(Managers.CM.LoadAndGetSound("PlayerShieldOn"));
	Player->SetThrustSound(Managers.CM.LoadAndGetSound("PlayerThrust"));
	Player->SetSpawnSound(Managers.CM.LoadAndGetSound("PlayerSpawn"));
	Player->SetMineDropSound(Managers.CM.LoadAndGetSound("HomingMineDrop"));
	Player->SetMineExplodeSound(Managers.CM.LoadAndGetSound("HomingMineExplode"));
	Player->SetDoubleShotSound(Managers.CM.LoadAndGetSound("DoubleShotFire"));
	Player->SetBigShotSound(Managers.CM.LoadAndGetSound("BigShotFire"));
	Player->SetPlasmaShotSound(Managers.CM.LoadAndGetSound("PlayerPlasmaFire"));

	Enemies->SetRockExplodeSound(Managers.CM.LoadAndGetSound("RockExplode"));
	Enemies->SetUFOExplodeSound(Managers.CM.LoadAndGetSound("UFOExplode"));
	Enemies->SetUFOFireSound(Managers.CM.LoadAndGetSound("UFOFire"));
	Enemies->SetUFOBigSound(Managers.CM.LoadAndGetSound("UFOBig"));
	Enemies->SetUFOSmallSound(Managers.CM.LoadAndGetSound("UFOSmall"));
	Enemies->SetDeathStarSpawnSound(Managers.CM.LoadAndGetSound("DeathStarSpawn"));
	Enemies->SetDeathStarExplodeSound(Managers.CM.LoadAndGetSound("DeathStarExplode"));
	Enemies->SetEnemyOneSpawnSound(Managers.CM.LoadAndGetSound("EnemyOneSpawn"));
	Enemies->SetEnemyOneOnSound(Managers.CM.LoadAndGetSound("EnemyOne"));
	Enemies->SetEnemyOneExplodeSound(Managers.CM.LoadAndGetSound("EnemyOneExplode"));
	Enemies->SetEnemyOneFireSound(Managers.CM.LoadAndGetSound("MissileLaunch"));
	Enemies->SetEnemyOneMissileOnSound(Managers.CM.LoadAndGetSound("MissileOn"));
	Enemies->SetEnemyOneMissileExplodeSound(Managers.CM.LoadAndGetSound("MissileExplode"));
	Enemies->SetEnemyTwoExplodeSound(Managers.CM.LoadAndGetSound("EnemyTwoExplode"));
	Enemies->SetEnemyTwoSpawnSound(Managers.CM.LoadAndGetSound("EnemyTwoSpawn"));
	Enemies->SetEnemyTwoFireSound(Managers.CM.LoadAndGetSound("MineDrop"));
	Enemies->SetEnemyTwoOnSound(Managers.CM.LoadAndGetSound("EnemyTwo"));
	Enemies->SetBossExplodeSound(Managers.CM.LoadAndGetSound("BossExplode"));
	Enemies->SetBossShieldHitSound(Managers.CM.LoadAndGetSound("BossShieldHit"));
	Enemies->SetBossShieldDownSound(Managers.CM.LoadAndGetSound("BossShieldDown"));
	Enemies->SetBossHitSound(Managers.CM.LoadAndGetSound("BossHitSound"));
	Enemies->SetBossTurretFireSound(Managers.CM.LoadAndGetSound("BossTurretFire"));
	Enemies->SetBossTurretExplodeSound(Managers.CM.LoadAndGetSound("BossTurretExplode"));
	Enemies->SetBossSpineFireSound(Managers.CM.LoadAndGetSound("BossSpineFire"));

	Logic->SetPowerUpSound(Managers.CM.LoadAndGetSound("PowerUp"));
	Logic->SetPickUpSound(Managers.CM.LoadAndGetSound("PickUp"));

	LineModelPoints rockModels[4];

	rockModels[0] = Managers.CM.LoadAndGetLineModel("Rock1");
	rockModels[1] = Managers.CM.LoadAndGetLineModel("Rock2");
	rockModels[2] = Managers.CM.LoadAndGetLineModel("Rock3");
	rockModels[3] = Managers.CM.LoadAndGetLineModel("Rock4");

	Enemies->SetRockModels(rockModels);


	return true;
}

bool Game::BeginRun()
{
	Particles->SetManagers(Managers);
	Player->SetParticleManager(Particles);
	Enemies->SetParticleManager(Particles);

	//Any Entities added after this point need this method fired manually if needed.
	Managers.BeginRun();

	return true;
}

void Game::ProcessInput()
{
	if (IsGamepadAvailable(0) && CursorDisabled)
	{
		EnableCursor();
		SetMousePosition(GetScreenWidth(), -GetScreenHeight());
		CursorDisabled = false;
	}
	else if (!CursorDisabled && !IsGamepadAvailable(0))
	{
		DisableCursor();
		CursorDisabled = true;
	}

	GameInput();
	Managers.EM.Input();
}


void Game::Update(float deltaTime)
{
	if (Logic->State == Pause)	return;

	Managers.EM.Update(deltaTime);
}

void Game::Draw3D()
{
	BeginMode3D(TheCamera);
	//3D Drawing here.

	Managers.EM.Draw3D();

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
	Managers.EM.Draw2D();
}

void Game::GameInput()
{
	if (IsKeyPressed(KEY_P))
	{
		Paused = !Paused;
	}

	Logic->GameInput();
}