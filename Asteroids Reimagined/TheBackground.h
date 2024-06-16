#pragma once
#include "Globals.h"

class TheBackground : public Common
{
public:
	TheBackground();
	virtual ~TheBackground();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

private:


};

