#pragma once
#include <random>
#include <ctime>
#include "raylib.h"

class Utilities
{
public:
	Utilities();

	float GetRandomFloat(float min, float max);

private:
	std::random_device RandomDevice = {};
	std::mt19937_64 Random19937 = {};

};