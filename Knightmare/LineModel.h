#pragma once
#include "Entity.h"
#include "ContentManager.h"

class LineModel : public Entity
{
public:
	float Alpha = 255;
	Color ModelColor = WHITE;

	virtual bool BeginRun();
	virtual void Input();
	virtual void Update(float deltaTime);
	virtual void AlwaysUpdate(float deltaTime);
	virtual void Draw3D();

	LineModelPoints GetLineModel();
	std::vector<Vector3> GetModel();
	void SetModel(std::vector<Vector3> lines);
	void SetModel(LineModelPoints lines);
	void SetModel(LineModelPoints lines, float scale);

private:

	void DrawLines(std::vector <Vector3> points, Vector3 rotationAxis, Color color);
};