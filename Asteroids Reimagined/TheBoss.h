#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheMissile.h"
#include "TheMine.h"
#include "Shot.h"
#include "TheBossTurret.h"

class TheBoss : public LineModel
{
public:
	TheBoss();
	virtual ~TheBoss();

	bool PlayerHit = false;
	int WaveNumber = 0;

	Entity* FireShotAtPlayerArea = nullptr;

	TheBossTurret* Turrets[5] = { nullptr };
	std::vector<TheMissile*> Missiles;
	std::vector<TheMine*> Mines;
	std::vector<Shot*> Shots;

	void SetPlayer(ThePlayer* player);

	void SetShieldModel(std::vector<Vector3> model);
	void SetTurretModel(std::vector<Vector3> model);
	void SetLeftSpineMountModel(std::vector<Vector3> model);
	void SetRightSpineMountModel(std::vector<Vector3> model);
	void SetMissileModel(std::vector<Vector3> model);
	void SetShotModel(std::vector<Vector3> model);
	void SetMineModel(std::vector<Vector3> model);

	void SetOnSound(Sound sound);
	void SetExplodeSound(Sound sound);
	void SetShieldHitSound(Sound sound);
	void SetShieldDownSound(Sound sound);
	void SetHitSound(Sound sound);
	void SetTurretFireSound(Sound sound);
	void SetTurretExplodeSound(Sound sound);
	void SetSpineFireSound(Sound sound);
	void SetMissileFireSound(Sound sound);
	void SetMissileOnSound(Sound sound);
	void SetMissileExplodeSound(Sound sound);
	void SetMineDropSound(Sound sound);
	void SetMineExplodeSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Reset();
	void Spawn(Vector3 position, float rotation);
	void Hit();
	void Destroy();

	void Hit(Entity* entity, int damage);

private:
	bool AllTurretsDead;

	size_t MissileFireTimerID = 0;
	size_t MineDropTimerID = 0;
	size_t FireTimerID;

	int ShieldPower = 100;
	int NextWaypoint = 0;

	float ShieldPoints = 0.0f;
	float ShieldRegenRate = 1.0f;
	float FireTimerSetting = 0.0f;
	float HitPoints = 0.0f;

	Sound OnSound = {};
	Sound ShieldHitSound = {};
	Sound ShieldDownSound = {};
	Sound HitSound = {};
	Sound ExplodeSound = {};
	Sound TurretFireSound = {};
	Sound TurretExplodeSound = {};
	Sound SpineFireSound = {};
	Sound MissileFireSound = {};
	Sound MissileOnSound = {};
	Sound MissileExplodeSound = {};
	Sound MineDropSound = {};
	Sound MineExplodeSound = {};

	std::vector<Vector3> MissileModel;
	std::vector<Vector3> ShotModel;
	std::vector<Vector3> MineModel;
	std::vector<Vector3> TurretModel;

	LineModel* Shield = nullptr;
	LineModel* RightSpineMount = nullptr;
	LineModel* LeftSpineMount = nullptr;

	ThePlayer* Player = nullptr;

	std::vector<Vector3> Path;

	void HeadToNextWaypoint();
	void ChasePlayer();
	void HeadToEdge();
	void ReachedWaypoint();
	void CheckCollisions();
	void FireShots();
	void FireMissile();
	void DropMine();
	void ShieldHit(int damage);
	void ShieldDown(Entity* entity, int damage);
};
