#include "SharedMethods.h"

SharedMethods::SharedMethods()
{

}

SharedMethods::~SharedMethods()
{
}

void SharedMethods::UpdateRocks(std::vector<TheRock*> rocks)
{
	Rocks = rocks;
}

float SharedMethods::AimedShotAtNearbyRock(Vector3 position)
{
	bool noRocks = true;

	Vector3 closestRockPosition = { 0, 0, 0 };
	Vector3 closestRockVelocity = { 0, 0, 0 };
	float shortestDistance = 1000.0f;

	for (const auto &rock : Rocks)
	{
		if (rock->Enabled)
		{
			float distance = Vector3Distance(rock->Position, position);

			if (!ForUFO)
			{
				if (distance < shortestDistance)
				{
					shortestDistance = distance;
					closestRockPosition = rock->Position;
					closestRockVelocity = rock->Velocity;
				}
			}
			else
			{
				noRocks = false;
	
				if (distance < shortestDistance && rock->Size != TheRock::Small)
				{
					shortestDistance = distance;
					closestRockPosition = rock->Position;
					closestRockVelocity = rock->Velocity;
				}
			}
		}
	}

	if (noRocks)
	{
		return M.GetRandomRadian();
	}

	Vector3 compensation = GetVelocityFromAngleZ(GetAngleFromVectorsZ(position, closestRockVelocity),
		(shortestDistance * 0.015f));

	Vector3 aimedclosestRock = Vector3Add(closestRockPosition, Vector3Add(closestRockVelocity, compensation));

	//Vector3 aimedclosestRock = Vector3Add(closestRockPosition, closestRockVelocity);

	ClosestRockPosition = closestRockPosition;
	AimedShotPosition = aimedclosestRock;

	return GetAngleFromVectorsZ(position, aimedclosestRock);
}
