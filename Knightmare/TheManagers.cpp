#include "TheManagers.h"

TheManagers::TheManagers()
{
}

TheManagers::~TheManagers()
{
}

bool TheManagers::Initialize()
{
	CM.Initialize();
	EM.Initialize();

	return false;
}

bool TheManagers::BeginRun()
{
	CM.BeginRun();
	EM.BeginRun();

	return false;
}

bool TheManagers::SetCamera(Camera& camera)
{
	EM.SetCamera(camera);

	return false;
}

void TheManagers::Update(float deltaTime)
{

}