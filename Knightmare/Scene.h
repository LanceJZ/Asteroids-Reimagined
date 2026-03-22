#pragma once
#include "Common.h"

struct Scene : Common
{
	Scene();
	virtual ~Scene();

	bool Initialize();
	bool BeginRun();

	virtual void Input();
	virtual void Update(float deltaTime);
	virtual void Draw();
};

