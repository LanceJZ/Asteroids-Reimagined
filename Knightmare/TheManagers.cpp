#include "TheManagers.h"

TheManagers::TheManagers()
{
}

TheManagers::~TheManagers()
{
}

bool TheManagers::SetCamera(Camera& camera)
{
	EM.SetCamera(camera);

	return true;
}

bool TheManagers::Initialize()
{
	CM.Initialize();
	EM.Initialize();

	return true;
}

bool TheManagers::BeginRun()
{
	CM.BeginRun();
	EM.BeginRun();

	return true;
}

void TheManagers::Update(float deltaTime)
{

}