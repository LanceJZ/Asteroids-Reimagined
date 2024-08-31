#pragma once
#include "Globals.h"
#include "Shot.h"
#include "TheHomingMine.h"
#include "ThePlasmaShot.h"
#include "TheScore.h"
#include "ParticleManager.h"

class ThePlayer : public LineModel
{
public:
	ThePlayer();
	virtual ~ThePlayer();

	bool NewLife = false;
	bool GameOver = false;
	bool Paused = false;

	int Lives { 0 };

	std::vector<Shot*> Shots = {};
	std::vector<Shot*> DoubleShots = {};
	std::vector<Shot*> BigShots = {};
	std::vector<TheHomingMine*> Mines = {};
	std::vector<ThePlasmaShot*> PlasmaShots = {};

	LineModel* Shield = {};

	void SetCrosshairModel(LineModelPoints model);
	void SetTurretModel(LineModelPoints model);
	void SetShotModel(LineModelPoints model);
	void SetFlameModel(LineModelPoints model);
	void SetShieldModel(LineModelPoints model);
	void SetTurretHeatModel(LineModelPoints model);
	void SetBigShotModel(LineModelPoints model);
	void SetMineModel(LineModelPoints model);

	void SetFireSound(Sound sound);
	void SetExplodeSound(Sound sound);
	void SetShieldOnSound(Sound sound);
	void SetShieldHitSound(Sound sound);
	void SetThrustSound(Sound sound);
	void SetSpawnSound(Sound sound);
	void SetBonusSound(Sound sound);
	void SetMineDropSound(Sound sound);

	void SetParticleManager(ParticleManager* particleManager);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Input();
	void Update(float deltaTime);
	void Draw3D();

	void Hit(Vector3 location, Vector3 velocity);
	void ScoreUpdate(int addToScore);
	void Spawn();
	void NewGame();
	void ExtraLife();
	void ShieldPowerUp();
	void GunPowerUp();
	void FullPowerUp();
	void BigShotPowerUp();
	void DoubleShotPowerUp();
	void MinePowerUp();
	void PlasmaShotPowerUp();

	int GetScore();
	void SetHighScore(int highScore);

private:
	bool TurretOverHeat = false;
	bool PoweredUp = false;
	bool PoweredUpRundown = false;
	bool ShieldOverCharge = false;
	bool GunOverCharge = false;

	size_t MagazineSize = 16;
	size_t FireRateTimerID = 0;
	size_t TurretCooldownTimerID = 0;
	size_t TurretHeatTimerID = 0;
	size_t PowerupTimerID = 0;
	size_t PowerupRundownTimerID = 0;
	size_t PowerUpBlinkTimerID = 0;

	int NextNewLifeScore = 10000;
	int TurretHeat = 0;
	int TurretHeatMax = 100;
	int BigShotCount = 0;
	int DoubleShotCount = 0;
	int MineCount = 0;
	int PlasmaShotCount = 0;
	int MissileCount = 0;

	float TurretDirection = 0.0f;
	float ShieldPower = 0.0f;
	float ShieldDrainRate = 0.0f;
	float ShieldRechargeRate = 0.0f;

	Sound FireSound = {};
	Sound ExplodeSound = {};
	Sound ShieldOnSound = {};
	Sound ShieldHitSound = {};
	Sound ThrustSound = {};
	Sound SpawnSound = {};
	Sound BonusSound = {};
	Sound MineDropSound = {};
	Sound MineExplodeSound = {};

	LineModel* Flame = {};
	LineModel* Turret = {};
	LineModel* Crosshair = {};
	LineModel* TurretHeatMeter = {};
	LineModelPoints ShotModel;
	LineModelPoints MineModel;
	LineModelPoints BigShotModel;

	TheScore* Score = {};
	ParticleManager* Particles = {};

	void PointTurret(float stickDirectionX, float stickDirectionY);
	void PointTurret(Vector3 mouseLocation);
	void FireTurret();
	void TurretTimers();
	void FireSecondary();

	void FireBigShot();
	void FireDoubleShot();
	void DropHomingMine();
	void FirePlasmaShot();

	void CrosshairUpdate();

	void RotateShip(float amount);
	void RotateStop();

	void ThrustOn(float amount);
	void ThrustOff();

	void ShieldOn();
	void ShieldOff();
	void ShieldPowerDrain(float deltaTime);

	void WeHaveThePower();

	void ShieldHit(Vector3 location, Vector3 velocity);
	void TurretHeatMeterUpdate();

	void Gamepad();
	void Keyboard();
};
