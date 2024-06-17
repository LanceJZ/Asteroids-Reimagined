#pragma once
#include "Globals.h"

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

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Input();
	void Update(float deltaTime);
	void Draw();

	void RotateLeft(float amount);
	void RotateRight(float amount);
	void RotateStop();

	void ThrustOn(float amount);
	void ThrustOff();

	void Hit();
	void ScoreUpdate(int addToScore);
	void Reset();
	void NewGame();

private:
	int NextNewLifeScore = 10000;

	void Gamepad();
	void Keyboard();
};
