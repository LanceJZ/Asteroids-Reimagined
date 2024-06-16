#pragma once
#include "Entity.h"

class Model3D : public Entity
{
public:
	bool Cull = true;
	float ModelScale = 1;
	Vector2 ViewableArea = { 0 };
	Vector3 ModelPosition = { 0 };
	Color ModelColor = WHITE;

	Model3D();
	~Model3D();

	bool Initialize(Utilities* utilities);
	void LoadModel(Model &model, Texture2D &texture);
	virtual bool BeginRun();
	virtual bool SetCamera(Camera* camera);
	virtual void Input();
	void Update(float deltaTime);
	void Draw();

	//void AddChild(Model3D* child);
	void SetModel(Model &model, float scale);
	void SetModel(Model &model);
	void SetModelCopy(Model model, float scale);
	Model& GetModel();
	Camera* GetCamera();
	void Spawn(Vector3 pos, Vector3 vel);
	void Unload();

private:
	Model TheModel = {};
	Camera* TheCamera3D = {};

	virtual void AddChildren(Model3D* child);
};

