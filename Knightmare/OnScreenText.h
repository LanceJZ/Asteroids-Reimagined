#pragma once
#include "Common.h"

class OnScreenText : public Common
{
public:
	OnScreenText();
	virtual ~OnScreenText();

	bool Initialize(Utilities* utilities);

	void FixedUpdate();
	void Draw2D();

private:


};

