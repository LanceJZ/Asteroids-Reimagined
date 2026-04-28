#pragma once
#include "Globals.h"
#include "ThePlayerControls.h"
#include "TheScoreOld.h"

class ThePlayer : public ThePlayerControls
{
public:
	ThePlayer();
	virtual ~ThePlayer();

	bool NewLife = false;
	bool Paused = false;
	bool AltMouseMode = false;

	void SetShotModel(std::vector<Vector3> model);
	void SetBigShotModel(std::vector<Vector3> model);
	void SetMineModel(std::vector<Vector3> model);

	void SetFireSound(Sound sound);
	void SetExplodeSound(Sound sound);
	void SetThrustSound(Sound sound);
	void SetBigShotSound(Sound sound);
	void SetDoubleShotSound(Sound sound);
	void SetCrosshairModel(std::vector<Vector3> model);
	void SetTurretHeatModel(std::vector<Vector3> model);

	void SetBonusSound(Sound sound);
	void SetGunHotSound(Sound sound);
	void SetShieldLowSound(Sound sound);
	void SetPowerUpWarningSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Input();
	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Hit();
	void ShieldHit(Vector3 position, Vector3 velocity, float power);
	void SetAntiPlayer(Entity* entity);
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
	bool MouseTurnedOff = false;

	size_t PowerUpBlinkTimerID = 0;

	int NextNewLifeScore = 10000;
	int MouseWheelScroll = 0;

	Sound BonusSound = {};
	Sound ShieldLowSound = {};
	Sound GunHotSound = {};
	Sound PowerUpWarningSound = {};

	Entity* AntiPlayer = nullptr;

	LineModel* Crosshair = {};
	LineModel* TurretHeatMeter = {};
	LineModel* AmmoMeter = {};
	LineModel* WeaponTypeIconBig = {};
	LineModel* WeaponTypeIconDoubleLeft = {};
	LineModel* WeaponTypeIconDoubleRight = {};
	LineModel* WeaponTypeIconPlasma = {};
	LineModel* WeaponTypeIconMine = {};

	TheScoreOld* Score = {};

	std::vector<LineModel*> AmmoMeterModels = {};

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
	void ThrustOff(float amount);

	void ShieldOn();
	void ShieldOff();
	void ShieldPowerDrain(float deltaTime);

	void WeHaveThePower();

	void TurretHeatMeterUpdate();
	void AmmoMeterUpdate(int ammoCount);
	void AddAmmoMeterModels(int count);
	void WeaponPlasmaIconUpdate(float deltaTime);
	void SwitchSecondaryWeapon(SecondaryWeaponType type);
	void IsSecondaryWeaponSwitched(float next);

	void Gamepad();
	void Keyboard();
	void Mouse();
	void MouseWheel();
	void AltMouse();
};
