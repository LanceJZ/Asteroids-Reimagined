#pragma once
#include "Globals.h"
#include "TheRock.h"

class SharedMethods : Common
{
public:
	SharedMethods();
	virtual ~SharedMethods();

	bool ForUFO = false;
	bool NoRocks = false;

	Vector3 ClosestRockPosition;
	Vector3 AimedShotPosition;
	void UpdateRocks(std::vector<TheRock*> rocks);

	float AimedShotAtNearbyRock(Vector3 position);

private:
	std::vector<TheRock*> Rocks;

};

