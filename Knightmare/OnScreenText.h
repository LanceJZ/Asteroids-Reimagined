#pragma once
#include "Common.h"

class OnScreenText : public Common
{
public:
	OnScreenText();
	virtual ~OnScreenText();

	virtual bool Initialize(Utilities* utilities);

	virtual void FixedUpdate();
	virtual void Draw2D();

private:


};

