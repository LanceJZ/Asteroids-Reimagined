#include "Managers.h"

Managers::Managers()
{
}

Managers::~Managers()
{
}

bool Managers::Initialize()
{
	CM.Initialize();
	EM.Initialize();

	return false;
}

bool Managers::BeginRun()
{
	CM.BeginRun();
	EM.BeginRun();

	return false;
}

bool Managers::SetCamera(Camera& camera)
{
	EM.SetCamera(camera);

	return false;
}

void Managers::Update(float deltaTime)
{

}