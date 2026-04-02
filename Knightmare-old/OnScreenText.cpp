#include "OnScreenText.h"

OnScreenText::OnScreenText()
{

}

OnScreenText::~OnScreenText()
{
}

bool OnScreenText::Initialize()
{
	Common::Initialize();

	return false;
}

void OnScreenText::FixedUpdate()
{
	Common::FixedUpdate();

}

void OnScreenText::Draw2D()
{
	if (!Enabled) return;

	Common::Draw2D();

}
