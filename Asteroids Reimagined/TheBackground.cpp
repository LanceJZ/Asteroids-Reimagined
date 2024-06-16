#include "TheBackground.h"

TheBackground::TheBackground()
{

}

TheBackground::~TheBackground()
{
}

bool TheBackground::Initialize(Utilities* utilities)
{
	Common::Initialize(utilities);

	return false;
}

bool TheBackground::BeginRun()
{

	return false;
}

void TheBackground::Update()
{
	Common::Update();

}