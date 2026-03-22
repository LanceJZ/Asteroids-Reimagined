#pragma once
#include "Entity.h"

class Model3D : public Entity
{
public:
	Vector2 HalfViewableArea = { 0 };
	Vector3 ModelPosition = { 0 };
	Color ModelColor = WHITE;

	Model3D();
	~Model3D();

	bool SetCamera(Camera* camera);
	void SetModelWithTexture(Model &model, Texture2D &texture);
	void Input();
	bool Initialize();
	bool BeginRun();
	void Update(float deltaTime);
	void AlwaysUpdate(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();
	void Draw2D();

	void SetModel(Model &model, float scale);
	void SetModel(Model &model);
	void SetModelCopy(Model model, float scale);

	Camera* GetCamera();

	void Spawn(Vector3 pos);
	void Spawn(Vector3 pos, Vector3 vel);
	void Unload();

private:
};

