#include "Common.h"

bool Common::Initialize(Utilities* utilities)
{
	GameWindowHalfHeight = GetScreenHeight() * 0.5f;
	GameWindowHalfWidth = GetScreenWidth() * 0.5f;

	TheUtilities = utilities;

	return false;
}

bool Common::BeginRun()
{

	return true;
}

void Common::Update()
{
}

void Common::Draw2D()
{
}

float Common::GetRandomScreenY()
{
	return GetRandomFloat(-GameWindowHalfHeight, GameWindowHalfHeight);
}

float Common::GetRandomScreenX()
{
	return GetRandomFloat(-GameWindowHalfWidth, GameWindowHalfWidth);
}

float Common::GetRandomFloat(float min, float max)
{
	return TheUtilities->GetRandomFloat(min, max);
}

float Common::GetRandomRadian()
{
	return GetRandomFloat(0, PI * 2);
}

float Common::RotateTowardsTargetZ(Vector3& origin, Vector3& target,
	float facingAngle, float magnitude)
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

Vector3 Common::GetRandomVelocity(float speed)
{
	return GetVelocityFromAngleZ(GetRandomRadian(), speed);
}

Vector3 Common::GetRandomVelocity(float speed, float radianDirection)
{
	return GetVelocityFromAngleZ(radianDirection, GetRandomFloat(speed * 0.15f, speed));
}

Vector3 Common::GetVelocityFromAngleZ(float rotation, float magnitude)
{
	return { cosf(rotation) * magnitude, sinf(rotation) * magnitude, 0 };
}

Color Common::GetRandomColor()
{
	return {(unsigned char)GetRandomValue(0, 255),
		(unsigned char)GetRandomValue(0, 255),
		(unsigned char)GetRandomValue(0, 255), 255};
}

Quaternion Common::EulerToQuaternion(float yaw, float pitch, float roll)
{
	float quaternionX, quaternionY, quaternionZ, quaternionW;
	float yawDev = yaw / 2, pitchDev = pitch / 2, rollDev = roll / 2;

	quaternionX = sinf(rollDev) * cosf(pitchDev) * cosf(yawDev) -
		cosf(rollDev) * sinf(pitchDev) * sinf(yawDev);
	quaternionY = cosf(rollDev) * sinf(pitchDev) * cosf(yawDev) +
		sinf(rollDev) * cosf(pitchDev) * sinf(yawDev);
	quaternionZ = cosf(rollDev) * cosf(pitchDev) * sinf(yawDev) -
		sinf(rollDev) * sinf(pitchDev) * cosf(yawDev);
	quaternionW = cosf(rollDev) * cosf(pitchDev) * cosf(yawDev) +
		sinf(rollDev) * sinf(pitchDev) * sinf(yawDev);

	return {quaternionX, quaternionY, quaternionZ, quaternionW};
}
