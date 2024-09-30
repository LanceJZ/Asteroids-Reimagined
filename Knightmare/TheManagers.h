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

	bool Initialize();
	bool BeginRun();
	bool SetCamera(Camera& camera);

	virtual void Update(float deltaTime);

private:

};