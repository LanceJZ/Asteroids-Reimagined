#pragma once
#include "PositionedObject.h"

class Entity : public PositionedObject
{
public:
	bool EntityOnly = false;
	bool ShowCollision = false;
	bool HideCollision = false;

	std::string EntityName = {};

	virtual bool Initialize(Utilities* utilities);
	virtual void Update(float deltaTime);
	virtual void Draw3D();

	void SetScale(float scale);

	virtual void Spawn(Vector3 position);
	virtual void Hit();
	virtual void Destroy();

	bool GetBeenHit();

	bool CirclesIntersect(Vector3 targetPosition, float targetRadius);
	bool CirclesIntersect(Entity& target);
	bool CirclesIntersectBullet(Entity& target);

protected:

private:
	bool BeenHit = false;
	Ray TheRay = { 0 };
	RayCollision TheRayCollision = { 0 };
};

