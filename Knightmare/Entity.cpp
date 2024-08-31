#include "Entity.h"
#include "rlgl.h"

bool Entity::Initialize(Utilities* utilities)
{
	PositionedObject::Initialize(utilities);

	return true;
}

void Entity::Update(float deltaTime)
{
	if (!Enabled) return;

	PositionedObject::Update(deltaTime);
}

void Entity::Draw3D()
{
#ifdef _DEBUG
	if((Enabled && !IsChild && !HideCollision) || EntityOnly || ShowCollision)
		DrawCircle3D(GetWorldPosition(), Radius * Scale, {0}, 0, {150, 50, 200, 200});
#endif
}

void Entity::SetScale(float scale)
{
	Scale = scale;
}

void Entity::Spawn(Vector3 position)
{
	Position = position;
	Enabled = true;
	BeenHit = false;
}

void Entity::Hit()
{
	BeenHit = true;
}

void Entity::Destroy()
{
	Enabled = false;
	BeenHit = false;
}

bool Entity::GetBeenHit()
{
	return BeenHit;
}

/// <summary>
/// Circle collusion detection. Target circle will be compared to this class's.
/// Will return true of they intersect. Only for use with 2D Z plane.
/// Only X and Y will be used.
/// </summary>
/// <param name="targetPosition">Target Vector3 position.</param>
/// <param name="targetRadius">Target float radius.</param>
/// <returns>bool</returns>
bool Entity::CirclesIntersect(Vector3 targetPosition, float targetRadius)
{
	if (!Enabled) return false;

	Vector2 distance = { targetPosition.x - Position.x,
		targetPosition.y - Position.y };

	float radius = (Radius * Scale) + targetRadius;

	if ((distance.x * distance.x) + (distance.y * distance.y)
		< radius * radius) return true;

	return false;
}

/// <summary>
/// Circle collusion detection. Target circle will be compared to this class's.
/// Will return true of they intersect. Only for use with 2D Z plane.
/// Only X and Y will be used.
/// </summary>
/// <param name="target">Target Entity.</param>
/// <returns>bool</returns>
bool Entity::CirclesIntersect(Entity& target)
{
	if (!target.Enabled) return false;

	return CirclesIntersect(target.Position, (target.Radius * target.Scale));
}

bool Entity::CirclesIntersectBullet(Entity& target)
{
	if (!Enabled || !target.Enabled)
		return false;

	TheRay.position = Position;

	if (Velocity.x > 0)
	{
		TheRay.direction.x = 1;
	}
	else if (Velocity.x < 0)
	{
		TheRay.direction.x = -1;
	}

	if (Velocity.y > 0)
	{
		TheRay.direction.y = 1;
	}
	else if (Velocity.y < 0)
	{
		TheRay.direction.y = -1;
	}

	TheRayCollision = GetRayCollisionSphere(TheRay,	target.Position, target.Radius);

	if (TheRayCollision.hit)
	{
		float distance = (Position.x - LastFramePosition.x) +
			(Position.y - LastFramePosition.y);

		if (distance < 0) distance *= -1;

		if (TheRayCollision.distance > 0)
		{
			if (TheRayCollision.distance > distance + Radius) return false;
		}
		else
		{
			if (TheRayCollision.distance * -1 > distance + Radius) return false;
		}

		return true;
	}

	return false;
}