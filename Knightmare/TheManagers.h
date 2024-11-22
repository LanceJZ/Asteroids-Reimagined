#pragma once
#include "ContentManager.h"
#include "EntityManager.h"

class TheManagers : Common
{
public:
	ContentManager CM;
	EntityManager EM;

	TheManagers();
	virtual ~TheManagers();

	bool SetCamera(Camera& camera);

	bool Initialize();
	bool BeginRun();

	virtual void Update(float deltaTime);

private:

};