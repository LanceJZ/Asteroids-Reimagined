#include "TheStarShot.h"

TheStarShot::TheStarShot()
{
}

TheStarShot::~TheStarShot()
{
}

bool TheStarShot::Initialize()
{
	LineModel::Initialize();

	return false;
}

bool TheStarShot::BeginRun()
{
	LineModel::BeginRun();

	int numberOfstarPoints = 4;
	int numberOfVerts = 6 * numberOfstarPoints;
	float angPeakSeg = 2 * PI / (float)numberOfstarPoints;
	float DegToRad = angPeakSeg / 2.0f;
	float radius = 12.0f;

	std::vector<Vector3> starPoints;

	for (int i = 0; i < numberOfVerts; i++)
	{
		float DegInRad = i * DegToRad;

		if (i % 3 != 0)
		{
			starPoints.push_back({ radius * cos(DegInRad + (PI / 2)), radius * sin(DegInRad + (PI / 2)) });
			starPoints.push_back({ radius / 3 * cos(DegInRad + (PI / 2)), radius / 3 * sin(DegInRad + (PI / 2)) });
		}
	}

	SetModel(starPoints);

	return false;
}

void TheStarShot::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void TheStarShot::Draw3D()
{
	LineModel::Draw3D();
}

void TheStarShot::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void TheStarShot::Destroy()
{
	Entity::Destroy();

}
