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
	void LoadModel(Model &model, Texture2D &texture);
	virtual void Input();
	virtual bool Initialize(Utilities* utilities);
	virtual bool BeginRun();
	virtual void Update(double deltaTime);
	virtual void FixedUpdate(double deltaTime);
	virtual void Draw3D();

	//void AddChild(Model3D* child);
	void SetModel(Model &model, float scale);
	void SetModel(Model &model);
	void SetModelCopy(Model model, float scale);
	Camera* GetCamera();
	void Spawn(Vector3 pos, Vector3 vel);
	void Unload();

private:
};

