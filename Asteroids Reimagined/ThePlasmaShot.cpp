#include "ThePlasmaShot.h"

ThePlasmaShot::ThePlasmaShot()
{
	LifeTimerID = Managers.EM.AddTimer(5.0f);
}

ThePlasmaShot::~ThePlasmaShot()
{
}

bool ThePlasmaShot::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool ThePlasmaShot::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void ThePlasmaShot::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (Managers.EM.TimerElapsed(LifeTimerID))
	{
		Enabled = false;
	}

	if (OffScreen())
	{
		Enabled = false;
	}

	SetScale(Scale += (GrowthRate * deltaTime));
}

void ThePlasmaShot::Draw3D()
{
	LineModel::Draw3D();
}

void ThePlasmaShot::Spawn(Vector3 position, Vector3 velocity)
{
	Entity::Spawn(position);

	Velocity = velocity;

	Managers.EM.ResetTimer(LifeTimerID);

	SetScale(1.0f);
}

void ThePlasmaShot::Destroy()
{
	Entity::Destroy();

}
