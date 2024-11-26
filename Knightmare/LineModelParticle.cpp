#include "LineModelParticle.h"

LineModelParticle::LineModelParticle()
{
}

LineModelParticle::~LineModelParticle()
{
}

void LineModelParticle::SetEntityManager(EntityManager* entityManager)
{
	EM = entityManager;
}

bool LineModelParticle::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool LineModelParticle::BeginRun()
{
	LineModel::BeginRun();

	LifeTimerID = EM->AddTimer();
	Radius = 0.0f;

	return false;
}

void LineModelParticle::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (EM->TimerElapsed(LifeTimerID))
	{
		Enabled = false;
	}
}

void LineModelParticle::Draw3D()
{
	LineModel::Draw3D();
}

void LineModelParticle::Spawn(Vector3 position, Vector3 velocity,
	float rotationZ, float maxSpeed, float maxTime)
{
	LineModel::Spawn(position);

	EM->ResetTimer(LifeTimerID, GetRandomFloat(maxTime * 0.25f, maxTime));

	//Change so that the velocity goes out from position.
	//The position is the center of the line model that is exploding.
	Vector3 velocityX = GetRandomVelocity(GetRandomFloat(maxSpeed * 0.15f,
		maxSpeed * 0.5f));

	Vector3 velocityOut = GetVelocityFromAngleZ(rotationZ, maxSpeed * 0.5f);

	Velocity = (velocity * Vector3(0.1f, 0.1f, 0)) + velocityX + velocityOut;
	RotationZ = rotationZ;
}

void LineModelParticle::Destroy()
{
	Entity::Destroy();

}
