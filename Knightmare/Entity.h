#pragma once
#include "PositionedObject.h"

class Entity : public PositionedObject
{
public:
	bool BeenHit = false;
	bool EntityOnly = false;
	std::string EntityName = {};

	virtual bool Initialize(Utilities* utilities);
	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual void Spawn(Vector3 position);
	virtual void Hit();
	virtual void Destroy();

	bool CirclesIntersect(Entity& target);
	bool CirclesIntersectBullet(Entity& target);

private:
	Ray TheRay = { 0 };
	RayCollision TheRayCollision = { 0 };
};

