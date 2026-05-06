#include "Common.h"

bool Common::Initialize()
{
	WindowFullWidth = GetScreenWidth();
	WindowFullHeight = GetScreenHeight();
	WindowHalfHeight = (int)(WindowFullHeight * 0.5f);
	WindowHalfWidth = (int)(WindowFullWidth * 0.5f);

	return false;
}

bool Common::BeginRun()
{
	return false;
}

void Common::Input()
{
}

void Common::Update()
{
}

void Common::FixedUpdate()
{
}

void Common::Draw2D()
{
}

float Common::GetRotationTowardsTargetZ(Vector3& origin, Vector3& target, float facingAngle, float magnitude)
{
	float turnVelocity = 0;
	float targetAngle = GetAngleFromVectorsZ(origin, target);
	float targetLessFacing = targetAngle - facingAngle;
	float facingLessTarget = facingAngle - targetAngle;

	if (abs(targetLessFacing) > PI)
	{
		if (facingAngle > targetAngle)
		{
			facingLessTarget = (((PI *2) - facingAngle) + targetAngle) * -1;
		}
		else
		{
			facingLessTarget = ((PI * 2) - targetAngle) + facingAngle;
		}
	}

	if (facingLessTarget > 0)
	{
		turnVelocity = -magnitude;
	}
	else
	{
		turnVelocity = magnitude;
	}

	return turnVelocity;
}

float Common::GetAngleFromVectorsZ(Vector3& origin, Vector3& target)
{
	return { atan2f(target.y - origin.y, target.x - origin.x) };
}

Vector3& Common::GetVelocityFromAngleZ(float angle, float magnitude)
{
	Vector3 velocity = { cosf(angle) * magnitude, sinf(angle) * magnitude, 0 };

	return velocity;
}
