#pragma once
#include "Common.h"

class Scene : Common
{
public:
	Scene();
	virtual ~Scene();

	bool Initialize();
	bool BeginRun();

	virtual void Input();
	virtual void Update(float deltaTime);
	virtual void Draw();

private:

};

