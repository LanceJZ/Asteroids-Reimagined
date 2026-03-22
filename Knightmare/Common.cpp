#include "Common.h"

bool Common::Initialize()
{
	WindowFullWidth = GetScreenWidth();
	WindowFullHeight = GetScreenHeight();
	WindowHalfHeight = (int)(WindowFullHeight * 0.5f);
	WindowHalfWidth = (int)(WindowFullWidth * 0.5f);

	return false;
}

bool Common::BeginRun()
{
	return false;
}

void Common::Input()
{
}

void Common::Update()
{
}

void Common::FixedUpdate()
{
}

void Common::Draw2D()
{
}