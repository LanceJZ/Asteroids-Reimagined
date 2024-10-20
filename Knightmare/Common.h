#pragma once
#include <vector>
#include "raylib.h"
//#include "raymath.h"
#include "raymath_operators.h"
#include "Utilities.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

class Common //Version 2.0.0 of Knightmare Arcade Engine.
{
public:
	const float TwoPi = 6.283185307179586476925286766559f;
	const float HalfPi = 1.5707963267948966192313216916398f;
	float GameWindowHalfWidth = { 0 };
	float GameWindowHalfHeight = { 0 };

	Utilities* TheUtilities = {};

	virtual bool Initialize(Utilities* utilities);
	virtual bool BeginRun();
	virtual void Input();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Draw2D();

	float GetRandomScreenY();
	float GetRandomScreenX();
	float GetRandomFloat(float min, float max);
	float GetRandomRadian();
	float GetRotationTowardsTargetZ(Vector3& origin, Vector3& target, float facingAngle,
		float magnitude);
	float GetAngleFromVectorsZ(Vector3& origin, Vector3& target);
	Vector3 GetRandomVelocity(float speed);
	Vector3 GetRandomVelocity(float speed, float radianDirection);
	Vector3 GetVelocityFromAngleZ(float rotation, float magnitude);
	Color GetRandomColor();
	Quaternion EulerToQuaternion(float yaw, float pitch, float roll);

private:
};