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

	void SetPlayer(ThePlayer* player);

	void SetShieldModel(LineModelPoints model);
	void SetTurretModel(LineModelPoints model);
	void SetLeftSpineMountModel(LineModelPoints model);
	void SetRightSpineMountModel(LineModelPoints model);
	void SetMissileModel(LineModelPoints model);
	void SetShotModel(LineModelPoints model);
	void SetMineModel(LineModelPoints model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Reset();
	void Spawn(Vector3 position, float rotation);
	void Hit();
	void Destroy();

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

	LineModelPoints MissileModel;
	LineModelPoints ShotModel;
	LineModelPoints MineModel;
	LineModelPoints TurretModel;

	Entity* FireShotAtPlayerArea = nullptr;

	LineModel* Shield = nullptr;
	LineModel* RightSpineMount = nullptr;
	LineModel* LeftSpineMount = nullptr;
	ThePlayer* Player = nullptr;

	std::vector<Vector3> Path;
	std::vector<TheMissile*> Missiles;
	std::vector<TheMine*> Mines;
	std::vector<Shot*> Shots;

	TheBossTurret* Turrets[5] = { nullptr };

	void HeadToNextWaypoint();
	void ReachedWaypoint();
	void CheckCollisions();
	void FireShots();
};
