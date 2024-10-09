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

	int Wave = 0;

	Entity* FireShotAtPlayerArea = nullptr;

	TheBossTurret* Turrets[5] = { nullptr };
	std::vector<TheMissile*> Missiles;
	std::vector<TheMine*> Mines;
	std::vector<Shot*> Shots;

	void SetPlayer(ThePlayer* player);
	void SetParticleManager(ParticleManager* particleManager);

	void SetShieldModel(LineModelPoints model);
	void SetTurretModel(LineModelPoints model);
	void SetLeftSpineMountModel(LineModelPoints model);
	void SetRightSpineMountModel(LineModelPoints model);
	void SetMissileModel(LineModelPoints model);
	void SetShotModel(LineModelPoints model);
	void SetMineModel(LineModelPoints model);

	void SetExplodeSound(Sound sound);
	void SetShieldHitSound(Sound sound);
	void SetShieldDownSound(Sound sound);
	void SetHitSound(Sound sound);
	void SetTurretFireSound(Sound sound);
	void SetTurretExplodeSound(Sound sound);
	void SetSpineFireSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Reset();
	void Spawn(Vector3 position, float rotation);
	void Hit();
	void Destroy();

	void Hit(Entity* entity, int damage);

private:
	size_t MissileFireTimerID = 0;
	size_t MineDropTimerID = 0;
	size_t FireTimerID;

	int ShieldPower = 100;
	int NextWaypoint = 0;

	float ShieldPoints = 100.0f;
	float ShieldRegenRate = 1.0f;
	float FireTimerSetting = 0;
	float HitPoints = 100.0f;

	Sound ShieldHitSound = {};
	Sound ShieldDownSound = {};
	Sound HitSound = {};
	Sound ExplodeSound = {};
	Sound TurretFireSound = {};
	Sound TurretExplodeSound = {};
	Sound SpineFireSound = {};

	LineModelPoints MissileModel;
	LineModelPoints ShotModel;
	LineModelPoints MineModel;
	LineModelPoints TurretModel;

	LineModel* Shield = nullptr;
	LineModel* RightSpineMount = nullptr;
	LineModel* LeftSpineMount = nullptr;

	ParticleManager* Particles = nullptr;
	ThePlayer* Player = nullptr;

	std::vector<Vector3> Path;

	void HeadToNextWaypoint();
	void ReachedWaypoint();
	void CheckCollisions();
	void FireShots();
	void FireMissile();
	void DropMine();
	void ShieldHit(int damage);
	void ShieldDown(Entity* entity, int damage);
};
