#include "KnightMath.h"

KnightMath::KnightMath()
{
	Random19937.seed(time(nullptr));
}

KnightMath::~KnightMath()
{
}

bool KnightMath::Initialize()
{
	WindowWidth = GetScreenWidth();
	WindowHeight = GetScreenHeight();
	GameWindowHalfHeight = (int)(WindowHeight * 0.5f);
	GameWindowHalfWidth = (int)(WindowWidth * 0.5f);

	return false;
}

bool KnightMath::BeginRun()
{

	return false;
}

float KnightMath::GetRandomScreenY()
{
	return GetRandomFloat((float)-GameWindowHalfHeight, (float)GameWindowHalfHeight);
}

float KnightMath::GetRandomScreenX()
{
	return GetRandomFloat((float)-GameWindowHalfWidth, (float)GameWindowHalfWidth);
}

float KnightMath::GetRandomFloat(float min, float max)
{
	if (min > max)
	{
		TraceLog(LOG_INFO, "*****\n");
		TraceLog(LOG_INFO, "********* GetRandomFloat used with min more than max *********\n");
		return 0;
	}

	if (true)
	{
		//Old Random Number Generator.
		std::default_random_engine randEngine(RandomDevice());
		std::uniform_real_distribution<float> urdRand(min, max);

		return urdRand(randEngine);
	}
	else if (true)
	{
		std::uniform_real_distribution<> urd(min, max);

		return (float)urd(Random19937);
	}
	else
	{
		//raylib Random Number Generator.
		float rand = 0;
		float acc = 1000.0f;

		return (float)GetRandomValue((int)min * (int)acc,
			(int)max * (int)acc) / (int)acc;
	}
}

float KnightMath::GetRandomRadian()
{
	return GetRandomFloat(0, PI * 2);
}

float KnightMath::GetRotationTowardsTargetZ(Vector3& origin, Vector3& target, float facingAngle, float magnitude)
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

float KnightMath::GetAngleFromVectorsZ(Vector3& origin, Vector3& target)
{
	return { atan2f(target.y - origin.y, target.x - origin.x) };
}

Vector3 KnightMath::GetRandomVelocity(float speed)
{
	return GetVelocityFromAngleZ(GetRandomRadian(), speed);
}

Vector3 KnightMath::GetRandomVelocity(float speed, float radianDirection)
{
	return GetVelocityFromAngleZ(radianDirection,
		GetRandomFloat(speed * 0.15f, speed));
}

Vector3 KnightMath::GetVelocityFromAngleZ(float rotation, float magnitude)
{
	return { cosf(rotation) * magnitude, sinf(rotation) * magnitude, 0 };
}

Vector3 KnightMath::GetVelocityFromAngleZ(float magnitude)
{
	float ang = GetRandomFloat(0, PI * 2);

	return GetVelocityFromAngleZ(ang, magnitude);
}

Vector3 KnightMath::GetVelocityFromVectorsZ(Vector3& origin, Vector3& target,
	float magnitude)
{
	float angle = (atan2f(target.y - origin.y, target.x - origin.x));

	return { cosf(angle) * magnitude, sinf(angle) * magnitude, 0 };;
}

Color KnightMath::GetRandomColor()
{
	return {(unsigned char)GetRandomValue(0, 255),
		(unsigned char)GetRandomValue(0, 255),
		(unsigned char)GetRandomValue(0, 255), 255};
}

Quaternion KnightMath::EulerToQuaternion(float yaw, float pitch, float roll)
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
