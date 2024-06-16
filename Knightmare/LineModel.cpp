#include "LineModel.h"
#include "rlgl.h"

bool LineModel::BeginRun()
{
	return true;
}

void LineModel::Input()
{
}

void LineModel::Update(float deltaTime)
{
	if (Enabled) Entity::Update(deltaTime);

	ModelColor.a = (char)Alpha;
}

void LineModel::Draw()
{
	Entity::Draw();

	if (!Enabled)
	{
		return;
	}

	if (LinePoints.size() < 2)
	{
		return;
	}

	rlPushMatrix();

	if (IsChild)
	{
		for (auto parent : Parents)
		{
			rlTranslatef(parent->Position.x, parent->Position.y, Position.z);
			rlRotatef(parent->RotationX, 1, 0, 0);
			rlRotatef(parent->RotationY, 0, 1, 0);
			rlRotatef(parent->RotationZ, 0, 0, 1);
		}
	}

	rlTranslatef(Position.x, Position.y, Position.z);
	rlRotatef(RotationX, 1, 0, 0);
	rlRotatef(RotationY, 0, 1, 0);
	rlRotatef(RotationZ, 0, 0, 1);
	rlScalef(Scale, Scale, Scale);
	rlBegin(RL_LINES);
	rlColor4ub(ModelColor.r, ModelColor.g, ModelColor.b, ModelColor.a);

	for (int i = 0; i < LinePoints.size() - 1; i++)
	{
		rlVertex3f(LinePoints[i].x, LinePoints[i].y, LinePoints[i].z);
		rlVertex3f(
			LinePoints[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(i) + 1].x,
			LinePoints[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(i) + 1].y,
			LinePoints[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(i) + 1].z
		);
	}

	WorldMatrix = rlGetMatrixTransform();
	WorldPosition = { WorldMatrix.m12, WorldMatrix.m13, WorldMatrix.m14 };
	WorldRotation = QuaternionToEuler(QuaternionFromMatrix(WorldMatrix));

	rlPopMatrix();
	rlEnd();
}

LineModelPoints LineModel::GetLineModel()
{
	return Lines;
}

std::vector<Vector3> LineModel::GetModel()
{
	return LinePoints;
}

void LineModel::SetModel(std::vector<Vector3> lines)
{
	LinePoints = lines;
	Lines.linePoints = lines;
}

void LineModel::SetModel(LineModelPoints lines)
{
	Lines = lines;
	LinePoints = lines.linePoints;
}

void LineModel::DrawLines(std::vector <Vector3> points, Vector3 rotationAxis, Color color)
{
	if (points.size() >= 2)
	{
		rlPushMatrix();
		rlTranslatef(Position.x, Position.y, 0);
		rlRotatef(RotationZ * (float)(180.0f / PI), rotationAxis.x, rotationAxis.y, rotationAxis.z);
		rlBegin(RL_LINES);
		rlColor4ub(color.r, color.g, color.b, color.a);

		for (int i = 0; i < points.size() - 1; i++)
		{
			rlVertex3f(points[i].x, points[i].y, points[i].z);
			rlVertex3f(
				points[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(i) + 1].x,
				points[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(i) + 1].y,
				points[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(i) + 1].z
			);
		}

		rlEnd();
		rlPopMatrix();
	}
}

void LineModel::DrawLines(Color color)
{
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	for (int i = 0; i < LinePoints.size() - 1; i++)
	{
		rlVertex3f(LinePoints[i].x, LinePoints[i].y, LinePoints[i].z);
		rlVertex3f(
			LinePoints[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(i) + 1].x,
			LinePoints[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(i) + 1].y,
			LinePoints[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(i) + 1].z
		);
	}

	rlEnd();
}
