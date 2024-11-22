#pragma once
#include "Entity.h"

class Model3D : public Entity
{
public:
	bool Cull = true;
	Vector2 ViewableArea = { 0 };
	Vector3 ModelPosition = { 0 };
	Color ModelColor = WHITE;

	Model3D();
	~Model3D();

	bool SetCamera(Camera* camera);
	void SetModelWithTexture(Model &model, Texture2D &texture);
	void Input();
	bool Initialize(Utilities* utilities);
	bool BeginRun();
	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void SetModel(Model &model, float scale);
	void SetModel(Model &model);
	void SetModelCopy(Model model, float scale);

	Camera* GetCamera();

	void Spawn(Vector3 pos, Vector3 vel);
	void Unload();

private:
};

