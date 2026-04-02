#include "Model3D.h"

Model3D::Model3D()
{
}

Model3D::~Model3D()
{
	if (IsChild)
	{
		IsChild = false;
	}

	if (IsParent)
	{
		IsParent = false;
	}
}

bool Model3D::Initialize()
{
	Entity::Initialize();

	HalfViewableArea = { (float)WindowHalfWidth, (float)WindowHalfHeight };

	return false;
}

void Model3D::SetModelWithTexture(Model &model, Texture2D &texture)
{
	Entity::SetModelWithTexture(model, texture);
}

bool Model3D::BeginRun()
{
	Entity::BeginRun();

	return false;
}

bool Model3D::SetCamera(Camera* camera)
{
	TheCamera3D = camera;

	return false;
}

void Model3D::Input()
{
}

void Model3D::Update(float deltaTime)
{
	Entity::Update(deltaTime);
}

void Model3D::AlwaysUpdate(float deltaTime)
{
	Entity::AlwaysUpdate(deltaTime);
}

void Model3D::FixedUpdate(float deltaTime)
{
	Entity::FixedUpdate(deltaTime);
}

void Model3D::Draw3D()
{
	if (!Enabled) return;

	Entity::Draw3D();

	if (TheCamera3D == nullptr)
	{
		TraceLog(LOG_INFO, "***************\n");
		TraceLog(LOG_ERROR,
		"*****************  Camera Reference is missing! ******************\n");

		return;
	}

	if (Cull)
	{
		if (IsChild)
		{
			Vector3 parentTest = GetWorldPosition();

			if (TheCamera3D->position.x > parentTest.x + VerticesSize +
				HalfViewableArea.x || TheCamera3D->position.x < parentTest.x +
				-VerticesSize + -HalfViewableArea.x)
			{
				WasCulled = true;
				return;
			}

			if (TheCamera3D->position.y > parentTest.y + VerticesSize +
				HalfViewableArea.y || TheCamera3D->position.y < parentTest.y +
				-VerticesSize + -HalfViewableArea.y)
			{
				WasCulled = true;
				return;
			}
		}
		else
		{
			if (TheCamera3D->position.x > GetWorldPosition().x + VerticesSize +
				HalfViewableArea.x
				|| TheCamera3D->position.x < GetWorldPosition().x + -VerticesSize +
				-HalfViewableArea.x)
			{
				WasCulled = true;
				return;
			}

			if (TheCamera3D->position.y > GetWorldPosition().y + VerticesSize +
				HalfViewableArea.y ||
				TheCamera3D->position.y < GetWorldPosition().y + -VerticesSize +
				-HalfViewableArea.y)
			{
				WasCulled = true;
				return;
			}
		}
	}

	WasCulled = false;

	if (!Stationary)
	{
		BeforeCalculate();
		CalculateWorldVectors();

		DrawModel(TheModel, ModelPosition, ModelScale, ModelColor);	// Draw 3D model
		AfterCalculate();
	}
	else
	{
		DrawModel(TheModel, Position, Scale, ModelColor);// Draw stationary 3D model
	}
}

void Model3D::Draw2D()
{
	Entity::Draw2D();
}

void Model3D::SetModel(Model &model, float scale)
{
	if (model.meshes == nullptr) return;

	Entity::SetModel(model, scale);
}

void Model3D::SetModel(Model& model)
{
	Entity::SetModel(model);
}

void Model3D::SetModelCopy(Model model, float scale)
{
	Entity::SetModelCopy(model, scale);
}

Camera* Model3D::GetCamera()
{
	return TheCamera3D;
}

void Model3D::Spawn(Vector3 pos)
{
	Entity::Spawn(pos);
}

void Model3D::Spawn(Vector3 pos, Vector3 vel)
{
	Spawn(pos);
	Velocity = vel;
}

void Model3D::Unload()
{
	UnloadModel(TheModel);
}