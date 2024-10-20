#include "PositionedObject.h"
#include "rlgl.h"

PositionedObject::PositionedObject()
{
	Parents = new std::vector<PositionedObject*>;
}

PositionedObject::~PositionedObject()
{
	Parents->clear();
}

bool PositionedObject::Initialize(Utilities* utilities)
{
	Common::Initialize(utilities);

	WindowWidth = (float)(GetScreenWidth() * 0.5f);
	WindowHeight = (float)(GetScreenHeight() * 0.5f);

	return false;
}

void PositionedObject::Update(float deltaTime)
{
	LastFramePosition = Position;

	if (!IsChild)
	{
		LastFrameWorldPosition = Position;
	}

	Velocity = Vector3Add(Velocity, Acceleration);
	Position = Vector3Add(Vector3Multiply({ deltaTime, deltaTime, deltaTime },
		Velocity), Position);

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

float PositionedObject::Chase(PositionedObject Chasing)
{
	return 0.0f;
}

void PositionedObject::RotateTowardsTargetZ(Vector3& target, float magnitude)
{
	RotationZ = Common::GetRotationTowardsTargetZ(Position, target, RotationZ, magnitude);
}

float PositionedObject::AngleFromVectorZ(Vector3 target)
{
	return (atan2f(target.y - Position.y, target.x - Position.x));
}

float PositionedObject::AngleFromWorldVectorZ(Vector3 target)
{
	return (atan2f(target.y - GetWorldPosition().y, target.x - GetWorldPosition().x));
}

Vector3 PositionedObject::RandomVelocity(float magnitude)
{
	float ang = GetRandomFloat(0, PI * 2);

	return GetVelocityFromAngleZ(ang, magnitude);
}

Vector3 PositionedObject::VelocityFromAngleZ(float magnitude)
{
	return { cosf(RotationZ) * magnitude, sinf(RotationZ) * magnitude, 0 };
}

float PositionedObject::X()
{
	return Position.x;
}

float PositionedObject::Y()
{
	return Position.y;
}

float PositionedObject::Z()
{
	return Position.z;
}

Vector3 PositionedObject::AccelerationToMaxAtRotation(float accelerationAmount, float topSpeed)
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

Vector3 PositionedObject::GetWorldPosition()
{
	BeforeCalculate();
	CalculateWorldVectors();
	CalculateWorldSpace();
	Vector3 worldPosition = WorldPosition;
	AfterCalculate();

	return worldPosition;
}

//Sets Acceleration based on acceleration amount this frame,
//up to a max amount based on top speed.
void PositionedObject::SetAccelerationToMaxAtRotation(float accelerationAmount,
	float topSpeed)
{
	Acceleration = AccelerationToMaxAtRotation(accelerationAmount, topSpeed);
}

//Sets Acceleration down to zero over time based on deceleration amount.
void PositionedObject::SetAccelerationToZero(float decelerationAmount)
{
	if (Velocity.x > 0.01 || Velocity.y > 0.01 ||
		Velocity.x < -0.01 || Velocity.y < -0.01)
	{
		Acceleration = { (-Velocity.x * decelerationAmount) * DeltaTime,
		(-Velocity.y * decelerationAmount) * DeltaTime, 0 };
	}
	else
	{
		Velocity = { 0, 0, 0 };
	}
}

void PositionedObject::X(float x)
{
	Position.x = x;
}

void PositionedObject::Y(float y)
{
	Position.y = y;
}

void PositionedObject::Z(float z)
{
	Position.z = z;
}

void PositionedObject::SetParent(PositionedObject& parent)
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

void PositionedObject::RemoveParent(PositionedObject* parent)
{
	auto parentID = std::find(Parents->begin(), Parents->end(), parent);
	if (parentID != Parents->end()) Parents->erase(parentID);
}

void PositionedObject::ClearParents()
{
	Parents->clear();
	IsChild = false;
}

void PositionedObject::CheckScreenEdge()
{
	CheckScreenEdgeX();
	CheckScreenEdgeY();
}

void PositionedObject::CheckScreenEdgeX()
{
	if (X() > WindowWidth)
	{
		X(-WindowWidth);
	}

	if (X() < -WindowWidth)
	{
		X(WindowWidth);
	}
}

void PositionedObject::CheckScreenEdgeY()
{
	if (Y() > WindowHeight)
	{
		Y(-WindowHeight);
	}

	if (Y() < -WindowHeight)
	{
		Y(WindowHeight);
	}
}

bool PositionedObject::ScreenEdgeBoundY()
{
	bool hitBound = false;

	if (Y() > WindowHeight - VerticesSize)
	{
		Y(WindowHeight);
		hitBound = true;
	}
	else if (Y() < -WindowHeight + VerticesSize)
	{
		Y(-WindowHeight);
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

bool PositionedObject::ScreenEdgeBoundY(float topOffset, float bottomOffset)
{
	bool hitBound = false;

	if (Y() > WindowHeight - bottomOffset - VerticesSize)
	{
		Y(WindowHeight - bottomOffset - VerticesSize);
		hitBound = true;
	}
	else if (Y() < -WindowHeight + topOffset + VerticesSize)
	{
		Y(-WindowHeight + topOffset + VerticesSize);
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

bool PositionedObject::ScreenEdgeBoundX()
{
	bool hitBound = false;

	if (X() > WindowWidth - VerticesSize)
	{
		X(WindowWidth - VerticesSize);
		hitBound = true;
	}
	else if (X() < -WindowWidth + VerticesSize)
	{
		X(-WindowWidth + VerticesSize);
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

bool PositionedObject::ScreenEdgeBoundX(float leftOffset, float rightOffset)
{
	bool hitBound = false;

	if (X() > WindowWidth - rightOffset - VerticesSize)
	{
		X(WindowWidth - rightOffset);
		hitBound = true;
	}
	else if (X() < -WindowWidth + leftOffset - VerticesSize)
	{
		X(-WindowWidth + leftOffset);
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

bool PositionedObject::OffScreen()
{
	return OffScreenSide() || OffScreenTopBottom();
}

bool PositionedObject::OffScreenSide()
{
	if (X() > WindowWidth || X() < -WindowWidth)
	{
		return true;
	}

	return false;
}

bool PositionedObject::OffScreenTopBottom()
{
	if (Y() > WindowHeight || Y() < -WindowHeight)
	{
		return true;
	}

	return false;
}

void PositionedObject::LeavePlay(float turnSpeed, float speed)
{
	float stageLeft = 0;
	float stageDown = 0;

	if (Position.x > 0)
	{
		stageLeft = WindowWidth;
	}
	else
	{
		stageLeft = -WindowWidth;
	}

	if (Position.y > 0)
	{
		stageDown = WindowHeight;
	}
	else
	{
		stageDown = -WindowHeight;
	}

	Vector3 position = { stageLeft, stageDown, 0 };

	SetRotateVelocity(position, turnSpeed, speed);
}

void PositionedObject::SetRotateVelocity(Vector3& position, float turnSpeed, float speed)
{
	RotationVelocityZ = Common::GetRotationTowardsTargetZ(Position, position, RotationZ,
		turnSpeed);
	Velocity = GetVelocityFromAngleZ(RotationZ, speed);
}

void PositionedObject::CheckPlayfieldSidesWarp()
{
	if (X() > WindowWidth) X(-WindowWidth);

	if (X() < -WindowWidth) X(WindowWidth);
}

void PositionedObject::CheckPlayfieldSidesWarp(float left, float right)
{
	if (X() > WindowWidth * right)
	{
		X(-WindowWidth * left);
	}
	else if (X() < -WindowWidth * left)
	{
		X(WindowWidth * right);
	}
}

void PositionedObject::CheckPlayfieldHeightWarp(float top, float bottom)
{
	if (Y() > WindowHeight * bottom)
	{
		Y(-WindowHeight * top);
	}
	else if (Y() < -WindowHeight * top)
	{
		Y(WindowHeight * bottom);
	}
}

//Sets Rotation Velocity Z to Aim at the Target.
void PositionedObject::AimAtTargetZ(Vector3& target, float facingAngle, float magnitude)
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

void PositionedObject::SetRotationZFromVector(Vector3& target)
{
	RotationZ = GetAngleFromVectors(target);
}

float PositionedObject::GetAngleFromVectors(Vector3& target)
{
	return (atan2f(target.y - Position.y, target.x - Position.x));
}

void PositionedObject::SetHeading(Vector3& waypoint, float rotationSpeed)
{
	AimAtTargetZ(waypoint, RotationZ, rotationSpeed);
}

void PositionedObject::BeforeCalculate()
{
	rlPushMatrix();
}

//Must have Push then CalculateWorldVectors/Pop/End before/after.
void PositionedObject::CalculateWorldSpace()
{
	LastFrameWorldPosition = WorldPosition;
	WorldMatrix = rlGetMatrixTransform();
	WorldPosition = { WorldMatrix.m12, WorldMatrix.m13, WorldMatrix.m14 };
	WorldRotation = QuaternionToEuler(QuaternionFromMatrix(WorldMatrix));
}

// Must have Push/Pop/End before/after.
void PositionedObject::CalculateWorldVectors()
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

void PositionedObject::AfterCalculate()
{
	rlPopMatrix();
	rlEnd();
}

//Internal method.
float PositionedObject::RadianSpin(float radian)
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

//Internal method.
float PositionedObject::AddRotationVelAcc(float rotation, float rotationVelocity,
	float rotationAcceleration, float deltaTime)
{
	rotationVelocity += rotationAcceleration * deltaTime;
	rotation += rotationVelocity * deltaTime;

	return rotation;
}
