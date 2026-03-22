#pragma once
#include "raylib.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

class Common //Version 3.1.0 of Knightmare Arcade Engine.
{
public:
	bool Enabled = true;

	const float TwoPi = 6.283185307179586476925286766559f;
	const float HalfPi = 1.5707963267948966192313216916398f;

	int WindowFullWidth = 0;
	int WindowFullHeight = 0;
	int WindowHalfWidth = { 0 };
	int WindowHalfHeight = { 0 };

	virtual bool Initialize();
	virtual bool BeginRun();
	virtual void Input();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Draw2D();

private:
};