#pragma once
#include "Globals.h"
#include "Shot.h"
#include "TheHomingMine.h"
#include "ThePlasmaShot.h"

enum SecondaryWeaponType
{
	None,
	Big,
	Double,
	Plasma,
	Mine
};

class ThePlayerControls : public LineModel
{
public:
	ThePlayerControls();
	virtual ~ThePlayerControls();

	bool GameOver = false;

	int WaveNumber = 0;
	int Lives = 0;

	LineModel* Shield = {};

	SecondaryWeaponType SecondaryWeapon = SecondaryWeaponType::None;

	std::vector<Shot*> Shots = {};
	std::vector<Shot*> DoubleShots = {};
	std::vector<Shot*> BigShots = {};
	std::vector<TheHomingMine*> Mines = {};
	std::vector<ThePlasmaShot*> PlasmaShots = {};

	void SetTurretModel(std::vector<Vector3> model);
	void SetShotModel(std::vector<Vector3> model);
	void SetFlameModel(std::vector<Vector3> model);
	void SetShieldModel(std::vector<Vector3> model);
	void SetBigShotModel(std::vector<Vector3> model);
	void SetMineModel(std::vector<Vector3> model);

	void SetFireSound(Sound sound);
	void SetExplodeSound(Sound sound);
	void SetShieldOnSound(Sound sound);
	void SetShieldHitSound(Sound sound);
	void SetThrustSound(Sound sound);
	void SetSpawnSound(Sound sound);
	void SetBigShotSound(Sound sound);
	void SetDoubleShotSound(Sound sound);
	void SetPlasmaShotSound(Sound sound);
	void SetMineDropSound(Sound sound);
	void SetMineExplodeSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Hit();
	void ShieldHit(Vector3 position, Vector3 velocity);
	void Spawn();
	void NewGame();
	void Spawn(Vector3 position);
	void ShieldPowerUp();
	void GunPowerUp();
	void FullPowerUp();
	void BigShotPowerUp();
	void DoubleShotPowerUp();
	void MinePowerUp();
	void PlasmaShotPowerUp();
	void Destroy();

protected:
	bool TurretOverheat = false;
	bool TurretOverheatCooldown = false;
	bool TurretOverheatRundown = false;
	bool PoweredUp = false;
	bool PoweredUpRundown = false;
	bool ShieldOverCharge = false;
	bool GunOverCharge = false;

	size_t MagazineSize = 0;
	size_t FireRateTimerID = 0;
	size_t TurretCooldownTimerID = 0;
	size_t TurretHeatTimerID = 0;
	size_t PowerupTimerID = 0;
	size_t PowerupRundownTimerID = 0;
	size_t FlameColorTimerID = 0;

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
	float PowerUpTimerAmount = 0.0f;

	Sound FireSound = {};
	Sound ExplodeSound = {};
	Sound ShieldOnSound = {};
	Sound ShieldHitSound = {};
	Sound ThrustSound = {};
	Sound SpawnSound = {};
	Sound BigShotSound = {};
	Sound DoubleShotSound = {};
	Sound MineDropSound = {};
	Sound MineExplodeSound = {};
	Sound PlasmaShotSound = {};

	LineModel* Flame = {};
	LineModel* Turret = {};

	std::vector<Vector3> ShotModel;
	std::vector<Vector3> MineModel;
	std::vector<Vector3> BigShotModel;

	void PointTurret(Vector3 mouseLocation);
	void FireTurret();
	void TurretTimers();
	void FireSecondary();

	void FireBigShot();
	void FireDoubleShot();
	void DropHomingMine();
	void FirePlasmaShot();

	void RotateShip(float amount);
	void RotateStop();

	void ThrustOn(float amount);
	void ThrustOff(float amount);

	void ShieldOn();
	void ShieldOff();
	void ShieldPowerDrain(float deltaTime);

	void WeHaveThePower();

	void SwitchSecondaryWeapon(SecondaryWeaponType type);
	void IsSecondaryWeaponSwitched(float next);

private:


};
