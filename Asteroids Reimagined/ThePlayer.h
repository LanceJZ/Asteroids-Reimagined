#pragma once
#include "Globals.h"
#include "Shot.h"

class ThePlayer : public LineModel
{
public:
	ThePlayer();
	virtual ~ThePlayer();

	bool NewLife = false;
	bool GameOver = false;

	int Score { 0 };
	int HighScore { 0 };
	int Lives { 0 };

	Shot* Shots[8] = { nullptr };

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void SetCrosshairModel(LineModelPoints model);
	void SetTurretModel(LineModelPoints model);
	void SetShotModel(LineModelPoints model);
	void SetFlameModel(LineModelPoints model);
	void SetShieldModel(LineModelPoints model);

	void Input();
	void Update(float deltaTime);
	void Draw3D();

	void Hit();
	void ScoreUpdate(int addToScore);
	void Reset();
	void NewGame();

private:
	bool TurretOverHeat = false;
	size_t ShotTimerID = 0;
	size_t TurretCooldownTimerID = 0;
	size_t TurretHeatTimerID = 0;
	int NextNewLifeScore = 10000;
	int TurretHeat = 0;
	int TurretHeatMax = 100;
	float TurretDirection = 0.0f;

	LineModel* Flame = {};
	LineModel* Shield = {};
	LineModel* Turret = {};
	LineModel* Crosshair = {};

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

	void Gamepad();
	void Keyboard();
};
