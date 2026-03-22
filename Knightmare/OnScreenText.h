#pragma once
#include <string>
#include "Common.h"

class OnScreenText : public Common
{
public:
	OnScreenText();
	virtual ~OnScreenText();

	bool Enabled = false;

	bool Initialize();

	void FixedUpdate();
	void Draw2D();

private:


};

