#pragma once
#include "Globals.h"
#include "Shot.h"
#include "TheScore.h"
#include "ParticleManager.h"

class ThePlayer : public LineModel
{
public:
	ThePlayer();
	virtual ~ThePlayer();

	bool NewLife = false;
	bool GameOver = false;

	int Lives { 0 };

	std::vector<Shot*> Shots = {};

	LineModel* Shield = {};

	void SetCrosshairModel(LineModelPoints model);
	void SetTurretModel(LineModelPoints model);
	void SetShotModel(LineModelPoints model);
	void SetFlameModel(LineModelPoints model);
	void SetShieldModel(LineModelPoints model);
	void SetTurretHeatModel(LineModelPoints model);

	void SetFireSound(Sound sound);
	void SetExplodeSound(Sound sound);
	void SetShieldOnSound(Sound sound);
	void SetShieldHitSound(Sound sound);
	void SetThrustSound(Sound sound);
	void SetSpawnSound(Sound sound);
	void SetBonusSound(Sound sound);

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
	void HeatPowerUp();
	void FullPowerUp();
	int GetScore();

private:
	bool TurretOverHeat = false;
	bool PoweredUp = false;
	bool PoweredUpRundown = false;

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

	LineModel* Flame = {};
	LineModel* Turret = {};
	LineModel* Crosshair = {};
	LineModel* TurretHeatMeter = {};

	TheScore* Score = {};
	ParticleManager* Particles = {};

	void PointTurret(float stickDirectionX, float stickDirectionY);
	void PointTurret(Vector3 mouseLocation);
	void FireTurret();
	void TurretTimers();

	void CrosshairUpdate();

	void RotateLeft(float amount);
	void RotateRight(float amount);
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
