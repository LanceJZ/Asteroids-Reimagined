#include "Game.h"

Game::Game()
{
	//When adding classes to EM, must be pointer to heap,IE: Name = new Class().

	LogicID = EM.AddCommon(Logic = DBG_NEW GameLogic());
	//BackGroundID = EM.AddCommon(BackGround = DBG_NEW TheBackground());
	EnemiesID = EM.AddCommon(Enemies = DBG_NEW EnemyControl());
	PlayerID = EM.AddLineModel(Player = DBG_NEW ThePlayer());
}

Game::~Game()
{
}

bool Game::Initialize() //Initialize
{
	Common::Initialize();

	//BackGround->Initialize();
	Enemies->Initialize();
	Logic->Initialize();
	Player->Initialize();

	DisableCursor();

	float multi = 1.0f;
	FieldSize = { GetScreenWidth() * multi, (float)GetScreenHeight() };

	Logic->SetPlayer(Player);
	Logic->SetEnemies(Enemies);

	Enemies->SetPlayer(Player);

	//Any Entities added after this point need this method fired manually.

	return true;
}

bool Game::Load()
{
	Logic->Load();

	//Models
	size_t shotModelID = CM.LoadTheLineModel("Dot");
	Player->SetModel(CM.LoadAndGetLineModel("PlayerShip"));
	Player->SetFlameModel(CM.LoadAndGetLineModel("PlayerFlame"));
	Player->SetShieldModel(CM.LoadAndGetLineModel("PlayerShield"));
	Player->SetTurretModel(CM.LoadAndGetLineModel("PlayerTurret"));
	Player->SetShotModel(CM.GetLineModel(shotModelID));
	Player->SetCrosshairModel(CM.LoadAndGetLineModel("Cross"));
	Player->SetTurretHeatModel(CM.LoadAndGetLineModel("HeatMeter"));
	Player->SetBigShotModel(CM.LoadAndGetLineModel("PlayerBigShot"));
	Player->SetMineModel(CM.LoadAndGetLineModel("HomingMine"));

	Logic->SetPowerUpModel(CM.LoadAndGetLineModel("PowerUp"));

	Enemies->SetUFOModel(CM.LoadAndGetLineModel("UFO"));
	Enemies->SetShotModel(CM.GetLineModel(shotModelID));
	Enemies->SetWedgeModel(CM.LoadAndGetLineModel("Wedge"));
	Enemies->SetEnemy1Model(CM.LoadAndGetLineModel("Enemy1"));
	Enemies->SetEnemy2Model(CM.LoadAndGetLineModel("Enemy2"));
	Enemies->SetEnemyMissileModel(CM.LoadAndGetLineModel("Missile"));
	Enemies->SetEnemyMineModel(CM.LoadAndGetLineModel("Mine"));
	Enemies->SetBossModel(CM.LoadAndGetLineModel("BossShip"));
	Enemies->SetBossShieldModel(CM.LoadAndGetLineModel("BossShield"));
	Enemies->SetBossTurretModel(CM.LoadAndGetLineModel("BossTurret"));
	Enemies->SetLeftSpineMountModel(CM.LoadAndGetLineModel("LeftSpineGun"));
	Enemies->SetRightSpineMountModel(CM.LoadAndGetLineModel("RightSpineGun"));

	//Sounds
	Player->SetExplodeSound(CM.LoadAndGetSound("PlayerExplode"));
	Player->SetBonusSound(CM.LoadAndGetSound("BonusShip"));
	Player->SetFireSound(CM.LoadAndGetSound("PlayerFire"));
	Player->SetShieldHitSound(CM.LoadAndGetSound("PlayerShieldHit"));
	Player->SetShieldOnSound(CM.LoadAndGetSound("PlayerShieldOn"));
	Player->SetThrustSound(CM.LoadAndGetSound("PlayerThrust"));
	Player->SetSpawnSound(CM.LoadAndGetSound("PlayerSpawn"));
	Player->SetMineDropSound(CM.LoadAndGetSound("HomingMineDrop"));
	Player->SetMineExplodeSound(CM.LoadAndGetSound("HomingMineExplode"));
	Player->SetDoubleShotSound(CM.LoadAndGetSound("DoubleShotFire"));
	Player->SetBigShotSound(CM.LoadAndGetSound("BigShotFire"));
	Player->SetPlasmaShotSound(CM.LoadAndGetSound("PlayerPlasmaFire"));
	Player->SetGunHotSound(CM.LoadAndGetSound("GunHot"));
	Player->SetShieldLowSound(CM.LoadAndGetSound("ShieldLow"));
	Player->SetPowerUpWarningSound(CM.LoadAndGetSound("PowerWarning"));

	Logic->SetPowerUpSound(CM.LoadAndGetSound("PowerUp"));
	Logic->SetPickUpSound(CM.LoadAndGetSound("PickUp"));

	Enemies->SetRockExplodeSound(CM.LoadAndGetSound("RockExplode"));
	Enemies->SetUFOExplodeSound(CM.LoadAndGetSound("UFOExplode"));
	Enemies->SetUFOFireSound(CM.LoadAndGetSound("UFOFire"));
	Enemies->SetUFOBigSound(CM.LoadAndGetSound("UFOBig"));
	Enemies->SetUFOSmallSound(CM.LoadAndGetSound("UFOSmall"));
	Enemies->SetDeathStarSpawnSound(CM.LoadAndGetSound("DeathStarSpawn"));
	Enemies->SetDeathStarExplodeSound(CM.LoadAndGetSound("DeathStarExplode"));
	Enemies->SetEnemyOneSpawnSound(CM.LoadAndGetSound("EnemyOneSpawn"));
	Enemies->SetEnemyOneOnSound(CM.LoadAndGetSound("EnemyOne"));
	Enemies->SetEnemyOneExplodeSound(CM.LoadAndGetSound("EnemyOneExplode"));
	Enemies->SetEnemyOneFireSound(CM.LoadAndGetSound("MissileLaunch"));
	Enemies->SetEnemyOneMissileOnSound(CM.LoadAndGetSound("MissileOn"));
	Enemies->SetEnemyOneMissileExplodeSound(CM.LoadAndGetSound("MissileExplode"));
	Enemies->SetEnemyTwoExplodeSound(CM.LoadAndGetSound("EnemyTwoExplode"));
	Enemies->SetEnemyTwoSpawnSound(CM.LoadAndGetSound("EnemyTwoSpawn"));
	Enemies->SetEnemyTwoFireSound(CM.LoadAndGetSound("MineDrop"));
	Enemies->SetEnemyTwoMineExplodeSound(CM.LoadAndGetSound("MineExplode"));
	Enemies->SetEnemyTwoOnSound(CM.LoadAndGetSound("EnemyTwo"));
	Enemies->SetBossOnSound(CM.LoadAndGetSound("BossShip"));
	Enemies->SetBossExplodeSound(CM.LoadAndGetSound("BossExplode"));
	Enemies->SetBossShieldHitSound(CM.LoadAndGetSound("BossShieldHit"));
	Enemies->SetBossShieldDownSound(CM.LoadAndGetSound("BossShieldDown"));
	Enemies->SetBossHitSound(CM.LoadAndGetSound("BossHit"));
	Enemies->SetBossTurretFireSound(CM.LoadAndGetSound("BossTurretFire"));
	Enemies->SetBossTurretExplodeSound(CM.LoadAndGetSound("BossTurretExplode"));
	Enemies->SetBossSpineFireSound(CM.LoadAndGetSound("BossSpineFire"));

	std::vector<Vector3> rockModels[4];

	rockModels[0] = CM.LoadAndGetLineModel("Rock1");
	rockModels[1] = CM.LoadAndGetLineModel("Rock2");
	rockModels[2] = CM.LoadAndGetLineModel("Rock3");
	rockModels[3] = CM.LoadAndGetLineModel("Rock4");

	Enemies->SetRockModels(rockModels);

	return true;
}

bool Game::BeginRun()
{

	//Any Entities added after this point need this method fired manually if needed.

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
}


void Game::Update(float deltaTime)
{
	if (Logic->State == Pause)	return;

}

void Game::FixedUpdate(float deltaTime)
{
	if (Logic->State == Pause)	return;

}

void Game::Draw3D()
{
#ifdef _DEBUG
	int fsx = int(FieldSize.x * 0.5f);
	int fsy = int(FieldSize.y * 0.5f);

	DrawLine(-fsx, -fsy, fsx, -fsy, { DARKBLUE }); //Top.
	DrawLine(fsx, -fsy, fsx, fsy, { DARKBLUE }); //Right side.
	DrawLine(fsx, fsy - 1, -fsx, fsy - 1, { DARKBLUE }); //Bottom.
	DrawLine(-fsx + 1, fsy - 1, -fsx + 1, -fsy - 1, { DARKBLUE }); //Left side.
#endif
}

void Game::Draw2D()
{

}
// For Game Input when game is paused or not.
void Game::GameInput()
{
	Logic->GameInput();
}
