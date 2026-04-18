#include "Entity.h"
#include "rlgl.h"

Entity::Entity()
{
	Parents = new std::vector<Entity*>;
}

Entity::~Entity()
{
	Parents->clear();
}

bool Entity::Initialize()
{
	Common::Initialize();

	return true;
}

void Entity::Update(float deltaTime)
{
	if (!Enabled) return;
	if (Stationary) return;

	LastFramePosition = Position;

	if (!IsChild)
	{
		LastFrameWorldPosition = Position;
	}

	Position = Position + ((Velocity = Velocity + Acceleration) * deltaTime);

	if (!IsChild)
	{
		WorldPosition = Position;
	}

	RotationX = AddRotationVelAcc(RotationX, RotationVelocityX,
		RotationAccelerationX, deltaTime);
	RotationY = AddRotationVelAcc(RotationY, RotationVelocityY,
		RotationAccelerationY, deltaTime);
	RotationZ = AddRotationVelAcc(RotationZ, RotationVelocityZ,
		RotationAccelerationZ, deltaTime);

	RotationX = RadianSpin(RotationX);
	RotationY = RadianSpin(RotationY);
	RotationZ = RadianSpin(RotationZ);

	DeltaTime = deltaTime;
}

void Entity::AlwaysUpdate(float deltaTime)
{
}

void Entity::FixedUpdate(float deltaTime)
{
	Common::FixedUpdate();
}

void Entity::Draw3D()
{
#ifdef _DEBUG
	if((Enabled && !NoCollision && !HideCollision) || (Enabled && !EntityOnly && !NoCollision && !HideCollision))
	{
		DrawCircle3D(GetWorldPosition(), Radius * Scale,
			{0}, 0, {150, 50, 200, 200});
	}
#endif
}

void Entity::Draw2D()
{
	Common::Draw2D();
}

void Entity::X(float x)
{
	Position.x = x;
}

void Entity::Y(float y)
{
	Position.y = y;
}

void Entity::Z(float z)
{
	Position.z = z;
}

void Entity::SetScale(float scale)
{
	Scale = scale;
}

void Entity::Spawn()
{
	Enabled = true;
	BeenHit = false;
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
bool Entity::CirclesIntersect(Vector3& targetPosition, float targetRadius)
{
	if (!Enabled) return false;

	Vector3 distance = {};

	if (!IsChild) distance = Vector3Subtract(targetPosition, Position);
	else distance = Vector3Subtract(targetPosition, GetWorldPosition());

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
	if (!target.Enabled || !Enabled) return false;

	Vector3 targetPosition = targetPosition = target.Position;

	if (target.IsChild) targetPosition = target.GetWorldPosition();

	return CirclesIntersect(target.Position, (target.Radius * target.Scale));
}

bool Entity::CirclesIntersectBullet(Entity& target)
{
	if (!Enabled || !target.Enabled)
		return false;

	TheRay.position = Position;

	if (Velocity.x > 0)
	{
		TheRay.direction.x = 1.0f;
	}
	else if (Velocity.x < 0)
	{
		TheRay.direction.x = -1.0f;
	}

	if (Velocity.y > 0)
	{
		TheRay.direction.y = 1.0f;
	}
	else if (Velocity.y < 0)
	{
		TheRay.direction.y = -1.0f;
	}

	RayCollision rayCollision = GetRayCollisionSphere(TheRay,
		target.Position, (target.Radius * target.Scale));

	if (rayCollision.hit)
	{
		float distance = Vector3Distance(Position, LastFramePosition);

		if (distance < 0) distance *= -1;

		if (rayCollision.distance > 0)
		{
			if (rayCollision.distance > Radius)
			{
				return false;
			}
		}
		else
		{
			if (rayCollision.distance * -1 > Radius)
			{
				return false;
			}
		}

		//TODO: Check each point from the Last frame to This frame.

		for (int i = 0; i < distance; i++)
		{
			Vector3 point = { Position.x - LastFramePosition.x + i,
				Position.y - LastFramePosition.y + i, 0 };

			if (target.CirclesIntersect(point, Radius)) return true;
		}
	}

	return false;
}

bool Entity::ScreenEdgeBoundY()
{
	bool hitBound = false;

	if (Y() > WindowHalfHeight - VerticesSize)
	{
		Y((float)WindowHalfHeight);
		hitBound = true;
	}
	else if (Y() < -WindowHalfHeight + VerticesSize)
	{
		Y((float)-WindowHalfHeight);
		hitBound = true;
	}

	if (hitBound)
	{
		Acceleration.y = 0;
		Velocity.y = 0;

		return true;
	}

	return false;
}

bool Entity::ScreenEdgeBoundY(float topOffset, float bottomOffset)
{
	bool hitBound = false;

	float top = topOffset + Radius;
	float bottom = bottomOffset + Radius;

	if (Y() > WindowHalfHeight - bottom)
	{
		Y((float)WindowHalfHeight - bottom);
		hitBound = true;
	}
	else if (Y() < -WindowHalfHeight + top)
	{
		Y((float)-WindowHalfHeight + top);
		hitBound = true;
	}

	if (hitBound)
	{
		Acceleration.y = 0;
		Velocity.y = 0;

		return true;
	}

	return false;
}

bool Entity::ScreenEdgeBoundX()
{
	bool hitBound = false;

	if (X() > WindowHalfWidth - VerticesSize)
	{
		X((float)WindowHalfWidth - VerticesSize);
		hitBound = true;
	}
	else if (X() < -WindowHalfWidth + VerticesSize)
	{
		X((float)-WindowHalfWidth + VerticesSize);
		hitBound = true;
	}

	if (hitBound)
	{
		Acceleration.x = 0;
		Velocity.x = 0;

		return true;
	}

	return false;
}

bool Entity::ScreenEdgeBoundX(float leftOffset, float rightOffset)
{
	bool hitBound = false;

	if (X() > WindowHalfWidth - rightOffset - VerticesSize)
	{
		X((float)WindowHalfWidth - rightOffset);
		hitBound = true;
	}
	else if (X() < -WindowHalfWidth + leftOffset - VerticesSize)
	{
		X((float)-WindowHalfWidth + leftOffset);
		hitBound = true;
	}

	if (hitBound)
	{
		Acceleration.x = 0;
		Velocity.x = 0;

		return true;
	}

	return false;
}

bool Entity::IsOffScreen()
{
	return IsOffScreenSide() || IsOffScreenTopBottom();
}

bool Entity::IsOffScreenSide()
{
	if (X() - VerticesSize > WindowHalfWidth ||
		X() + VerticesSize < -WindowHalfWidth) return true;

	return false;
}

bool Entity::IsOffScreenTopBottom()
{
	if (Y() - VerticesSize > WindowHalfHeight ||
		Y() + VerticesSize < -WindowHalfHeight)	return true;

	return false;
}

float Entity::X()
{
	return Position.x;
}

float Entity::Y()
{
	return Position.y;
}

float Entity::Z()
{
	return Position.z;
}

float Entity::GetAngleFromVectorZ(Vector3& target)
{
	return (atan2f(target.y - Position.y, target.x - Position.x));
}

float Entity::GetAngleFromWorldVectorZ(Vector3& target)
{
	return (atan2f(target.y - GetWorldPosition().y, target.x - GetWorldPosition().x));
}

float Entity::GetAngleFromVectors(Vector3& target)
{
	return (atan2f(target.y - Position.y, target.x - Position.x));
}

float Entity::GetAngleFromVectorsZ(Vector3& origin, Vector3& target)
{
	return { atan2f(target.y - origin.y, target.x - origin.x) };
}

float Entity::GetRotationTowardsTargetZ(Vector3& origin, Vector3& target,
	float facingAngle, float magnitude)
{
	float turnVelocity = 0;
	float targetAngle = GetAngleFromVectorsZ(origin, target);
	float targetLessFacing = targetAngle - facingAngle;
	float facingLessTarget = facingAngle - targetAngle;

	if (abs(targetLessFacing) > PI)
	{
		if (facingAngle > targetAngle)
		{
			facingLessTarget = (((PI *2) - facingAngle) + targetAngle) * -1;
		}
		else
		{
			facingLessTarget = ((PI * 2) - targetAngle) + facingAngle;
		}
	}

	if (facingLessTarget > 0)
	{
		turnVelocity = -magnitude;
	}
	else
	{
		turnVelocity = magnitude;
	}

	return turnVelocity;
}

Vector3& Entity::GetVelocityFromAngleZ(float angle, float magnitude)
{
	Vector3 velocity = { cosf(angle) * magnitude, sinf(angle) * magnitude, 0 };

	return velocity;
}

Vector3& Entity::GetVelocityFromAngleZ(float magnitude)
{
	Vector3 velocity = { cosf(RotationZ) * magnitude, sinf(RotationZ) * magnitude, 0 };

	return velocity;
}

Vector3& Entity::GetVelocityFromVectorZ(Vector3& target, float magnitude)
{
	float angle = (atan2f(target.y - Position.y, target.x - Position.x));

	Vector3 velocity = { cosf(angle) * magnitude, sinf(angle) * magnitude, 0 };

	return velocity;
}

Vector3& Entity::GetAccelerationToMaxAtRotation(float accelerationAmount, float topSpeed)
{
	Vector3 acceleration = { 0, 0, 0 };

	float topSpeedX = (cosf(RotationZ) * Velocity.x) - topSpeed;
	float topSpeedY = (sinf(RotationZ) * Velocity.y) - topSpeed;

	acceleration = {
		(cosf(RotationZ) * -(topSpeedX - accelerationAmount)) * DeltaTime,
		(sinf(RotationZ) * -(topSpeedY - accelerationAmount)) * DeltaTime,
		0 };

	return acceleration;
}

Vector3& Entity::GetReflectionVelocity(Vector3& position,
	Vector3& velocity, float amountReflect,
	float reductionHit, float reductionLoss)
{
	Vector3 reflection = Vector3Add(Vector3Multiply(Vector3Multiply(Velocity,
		{reductionLoss}), {-1}),
		Vector3Add(Vector3Multiply(velocity, {reductionHit}),
			GetVelocityFromAngleZ(GetAngleFromVectorsZ(position, Position),
				amountReflect)));
	return reflection;
}

Vector3& Entity::GetWorldPosition()
{
	BeforeCalculate();
	CalculateWorldVectors();
	CalculateWorldSpace();
	Vector3 worldPosition = WorldPosition;
	AfterCalculate();

	return worldPosition;
}


void Entity::SetParent(Entity& parent)
{
	if (IsChild) return;

	for (auto &myParent : *parent.Parents)
	{
		Parents->push_back(myParent);
	}

	Parents->push_back(&parent);

	parent.IsParent = true;
	IsChild = true;
}

bool Entity::SetCamera(Camera* camera)
{
	TheCamera3D = camera;

	return true;
}

void Entity::SetModel(Model& model, float scale)
{
	if (model.meshes == nullptr || model.meshes->vertices == nullptr)
	{
		return;
	}

	TheModel = model;
	ModelScale = scale;

	if (!NoCollision)
	{
		VerticesSize = (*model.meshes->vertices * -1.0f) * scale;
		Radius = VerticesSize;
	}
}

void Entity::SetModel(Model& model)
{
	SetModel(model, 1.0f);
}

void Entity::SetModelCopy(Model model, float scale)
{
	TheModel = model;
	ModelScale = scale;
}

void Entity::SetModelWithTexture(Model& model, Texture2D& texture)
{
	//if (IsTextureReady(texture)) //From raylib 5.0, no longer supported in 5.5.
	//{
	//	TheModel = model;
	//	TheModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	//}

	//if (IsTextureValid(texture)) //For raylib 5.5, wont compile.
	//{
	//	TheModel = model;
	//	TheModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	//}

	TheModel = model;
	TheModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

std::vector<Vector3>& Entity::GetLineModel()
{
	return LinePoints;
}

void Entity::SetModel(std::vector<Vector3> &lines)
{
	LinePoints = lines;
	if (!NoCollision) CalculateRadius();
}

void Entity::SetModel(std::vector<Vector3> &lines, float scale)
{
	Scale = scale;
	SetModel(lines);
}

Model& Entity::Get3DModel()
{
	return TheModel;
}

void Entity::RemoveParent(Entity* parent)
{
	auto parentID = std::find(Parents->begin(), Parents->end(), parent);
	if (parentID != Parents->end()) Parents->erase(parentID);
}

void Entity::ClearParents()
{
	Parents->clear();
	IsChild = false;
}

void Entity::CheckScreenEdge()
{
	CheckScreenEdgeX();
	CheckScreenEdgeY();
}

void Entity::CheckScreenEdgeX()
{
	if (X() > WindowHalfWidth)
	{
		X((float) -WindowHalfWidth);
	}

	if (X() < -WindowHalfWidth)
	{
		X((float)WindowHalfWidth);
	}
}

void Entity::CheckScreenEdgeY()
{
	if (Y() > WindowHalfHeight)
	{
		Y((float)-WindowHalfHeight);
	}

	if (Y() < -WindowHalfHeight)
	{
		Y((float)WindowHalfHeight);
	}
}

void Entity::CheckPlayfieldSidesWarp()
{
	if (X() > (float)WindowHalfWidth) X((float)-WindowHalfWidth);

	if (X() < (float)-WindowHalfWidth) X((float)WindowHalfWidth);
}

bool Entity::CheckPlayfieldSidesWarp(float left, float right)
{
	bool warped = false;

	if (X() > WindowHalfWidth * right)
	{
		X((float)-WindowHalfWidth * left);
		warped = true;
	}
	else if (X() < -WindowHalfWidth * left)
	{
		X((float)WindowHalfWidth * right);
		warped = true;
	}

	return warped;
}

void Entity::CheckPlayfieldHeightWarp(float top, float bottom)
{
	if (Y() > WindowHalfHeight * bottom)
	{
		Y((float)-WindowHalfHeight * top);
	}
	else if (Y() < -WindowHalfHeight * top)
	{
		Y((float)WindowHalfHeight * bottom);
	}
}

void Entity::LeavePlay(float turnSpeed, float speed)
{
	float stageLeft = 0;
	float stageDown = 0;

	if (Position.x > 0)
	{
		stageLeft = (float)WindowHalfWidth;
	}
	else
	{
		stageLeft = (float)-WindowHalfWidth;
	}

	if (Position.y > 0)
	{
		stageDown = (float)WindowHalfHeight;
	}
	else
	{
		stageDown = (float)-WindowHalfHeight;
	}

	Vector3 position = { stageLeft, stageDown, 0 };

	SetRotateVelocity(position, turnSpeed, speed);
}

void Entity::Reset()
{
	Destroy();
	Position = { 0.0f, 0.0f, 0.0f };
	Acceleration = { 0.0f, 0.0f, 0.0f };
	Velocity = { 0.0f, 0.0f, 0.0f };
	RotationVelocityX = 0.0f;
	RotationVelocityY = 0.0f;
	RotationVelocityZ = 0.0f;
}

void Entity::SetAccelerationToMaxAtRotation(float accelerationAmount, float topSpeed)
{
	Acceleration = GetAccelerationToMaxAtRotation(accelerationAmount, topSpeed);
}

void Entity::SetAccelerationToZero(float decelerationAmount)
{
	if (Velocity.x > 0.01 || Velocity.y > 0.01 ||
		Velocity.x < -0.01 || Velocity.y < -0.01)
	{
		Acceleration = (Velocity * -decelerationAmount) * DeltaTime;
	}
	else
	{
		Velocity = { 0, 0, 0 };
	}
}

void Entity::SetRotateVelocity(Vector3& position, float turnSpeed, float speed)
{
	RotationVelocityZ = GetRotationTowardsTargetZ(Position, position, RotationZ,
		turnSpeed);
	Velocity = GetVelocityFromAngleZ(RotationZ, speed);
}

void Entity::SetRotationZFromVector(Vector3& target)
{
	RotationZ = GetAngleFromVectors(target);
}

void Entity::SetAimAtTargetZ(Vector3& target, float facingAngle, float magnitude)
{
	float turnVelocity = 0;
	float targetAngle = GetAngleFromVectors(target); //This is why it is here.
	float targetLessFacing = targetAngle - facingAngle;
	float facingLessTarget = facingAngle - targetAngle;

	if (abs(targetLessFacing) > PI)
	{
		if (facingAngle > targetAngle)
		{
			facingLessTarget = ((TwoPi - facingAngle) + targetAngle) * -1;
		}
		else
		{
			facingLessTarget = (TwoPi - targetAngle) + facingAngle;
		}
	}

	if (facingLessTarget > 0)
	{
		turnVelocity = -magnitude;
	}
	else
	{
		turnVelocity = magnitude;
	}

	RotationVelocityZ = turnVelocity;
}

void Entity::BeforeCalculate()
{
	rlPushMatrix();
}

//Must have Push then CalculateWorldVectors/Pop/End before/after.
void Entity::CalculateWorldSpace()
{
	LastFrameWorldPosition = WorldPosition;
	WorldMatrix = rlGetMatrixTransform();
	WorldPosition = { WorldMatrix.m12, WorldMatrix.m13, WorldMatrix.m14 };
	WorldRotation = QuaternionToEuler(QuaternionFromMatrix(WorldMatrix));
}

// Must have Push/Pop/End before/after.
void Entity::CalculateWorldVectors()
{
	if (IsChild)
	{
		for (auto &parent : *Parents)
		{
			rlTranslatef(parent->Position.x, parent->Position.y,
				parent->Position.z);

			if (!IgnoreParentRotation)
			{
				rlRotatef(parent->RotationX, 1, 0, 0);
				rlRotatef(parent->RotationY, 0, 1, 0);
				rlRotatef(parent->RotationZ, 0, 0, 1);
			}

			rlScalef(parent->Scale, parent->Scale, parent->Scale);
		}
	}

	rlTranslatef(Position.x, Position.y, Position.z);
	rlRotatef(RotationX, 1, 0, 0);
	rlRotatef(RotationY, 0, 1, 0);
	rlRotatef(RotationZ, 0, 0, 1);
	rlScalef(Scale, Scale, Scale);
}

void Entity::AfterCalculate()
{
	rlPopMatrix();
	rlEnd();
}

float Entity::RadianSpin(float radian)
{
	if (radian > PI * 2)
	{
		radian -= PI * 2;
	}
	else if (radian < 0)
	{
		radian += PI * 2;
	}

	return radian;
}

float Entity::AddRotationVelAcc(float rotation, float rotationVelocity,
	float rotationAcceleration, float deltaTime)
{
	rotationVelocity += rotationAcceleration * deltaTime;
	rotation += rotationVelocity * deltaTime;

	return rotation;
}

void Entity::CalculateRadius()
{
	float farDistance = 0.0f;

	if (LinePoints.size() < 2)
	{
		return;
	}

	for (int i = 0; i < LinePoints.size(); i++)
	{
		float distance = Vector3Distance(LinePoints[i], { 0 });

		if (distance > farDistance)
		{
			farDistance = distance;
		}
	}

	Radius = farDistance * 0.9f * Scale;
}
