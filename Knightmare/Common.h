#pragma once
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "Utilities.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))
#define TWOPI = 6.283185307179586476925286766559f;

class Common
{
public:
	const float TwoPi = 6.283185307179586476925286766559f;
	float GameWindowHalfWidth = { 0 };
	float GameWindowHalfHeight = { 0 };

	Utilities* TheUtilities = {};

	virtual bool Initialize(Utilities* utilities);
	virtual bool BeginRun();
	virtual void Update();
	virtual void Draw2D();
	float GetRandomScreenY();
	float GetRandomScreenX();
	float GetRandomFloat(float min, float max);
	float GetRandomRadian();
	float RotateTowardsTargetZ(Vector3& origin, Vector3& target, float facingAngle,
		float magnitude);
	float GetAngleFromVectorsZ(Vector3& origin, Vector3& target);
	Vector3 GetRandomVelocity(float speed);
	Vector3 GetRandomVelocity(float speed, float radianDirection);
	Vector3 GetVelocityFromAngleZ(float rotation, float magnitude);
	Color GetRandomColor();
	Quaternion EulerToQuaternion(float yaw, float pitch, float roll);

private:
};