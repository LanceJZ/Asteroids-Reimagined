#include "OnScreenText.h"

OnScreenText::OnScreenText()
{

}

OnScreenText::~OnScreenText()
{
}

bool OnScreenText::Initialize(Utilities* utilities)
{
	Common::Initialize(TheUtilities);

	return false;
}

void OnScreenText::FixedUpdate()
{
	Common::FixedUpdate();

}

void OnScreenText::Draw2D()
{

}
