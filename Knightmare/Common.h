#pragma once
#include <vector>
#include "raylib.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

class Common //Version 3.2.0 of Knightmare Arcade Engine.
{
public:
	bool Enabled = true;
	bool GameOver = false;

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

	float GetRotationTowardsTargetZ(Vector3& origin, Vector3& target, float facingAngle, float magnitude);
	float GetAngleFromVectorsZ(Vector3& origin, Vector3& target);
	Vector3& GetVelocityFromAngleZ(float angle, float magnitude);


private:
};