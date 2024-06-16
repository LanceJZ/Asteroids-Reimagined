#pragma once
#include "Common.h"

class PositionedObject : public Common
{
public:
	PositionedObject();
	virtual ~PositionedObject();

	bool IsChild = false;
	bool IsParent = false;
	bool IsConnectedChild = true;
	bool Enabled = true;
	bool IgnoreParentRotation = false;
	bool WasCulled = false;
	int ChildNumber = 0;
	float Scale = 1;
	float MaxSpeed = 0;
	float Radius = 0;
	float VerticesSize = 0;
	float WindowWidth = 0;
	float WindowHeight = 0;
	float RotationX = 0;
	float RotationY = 0;
	float RotationZ = 0;
	float RotationVelocityX = 0;
	float RotationVelocityY = 0;
	float RotationVelocityZ = 0;
	float RotationAccelerationX = 0;
	float RotationAccelerationY = 0;
	float RotationAccelerationZ = 0;

	Vector3 Acceleration = Vector3Zero();
	Vector3 Velocity = Vector3Zero();
	Vector3 Position = Vector3Zero();
	Vector3 Rotation = Vector3Zero();
	Vector3 RotationVelocity = Vector3Zero();
	Vector3 RotationAcceleration = Vector3Zero();
	Vector3 LastFramePosition = Vector3Zero();
	Vector3 LastFrameWorldPosition = Vector3Zero();
	Vector3 WorldPosition = Vector3Zero();
	Vector3 WorldRotation = Vector3Zero();
	Quaternion RotationQ = {};
	Matrix WorldMatrix = MatrixIdentity();
	std::vector<PositionedObject*> Parents;

	virtual bool Initialize(Utilities* utilities);
	virtual void Update(float deltaTime);

	virtual float X();
	virtual float Y();
	virtual float Z();
	virtual void X(float x);
	virtual void Y(float y);
	virtual void Z(float z);

	bool ScreenEdgeBoundY();
	bool ScreenEdgeBoundY(float topOffset, float bottomOffset);
	bool ScreenEdgeBoundX();
	bool ScreenEdgeBoundX(float leftOffset, float rightOffset);
	bool OffScreen();
	bool OffScreenSide();
	bool OffScreenTopBottom();
	float Chase(PositionedObject Chasing);
	float AngleFromVectorZ(Vector3 target);
	float GetAngleFromVectors(Vector3& target);
	Vector3 RandomVelocity(float magnitude);
	Vector3 VelocityFromAngleZ(float magnitude);
	Vector3 DecelerationToZero(float decelerationAmount, float deltaTime);

	void RotateTowardsTargetZ(Vector3& target, float magnitude);
	void SetAccelerationToMaxAtRotation(float accelerationAmount, float topSpeed,
		float deltaTime);
	void SetParent(PositionedObject* parent);
	void RemoveFromParents();
	void DisconnectChild(PositionedObject* child);
	void ConnectChild(PositionedObject* child);
	void CheckScreenEdge();
	void CheckScreenEdgeX();
	void CheckScreenEdgeY();
	void LeavePlay(float turnSpeed, float speed);
	void RotateVelocity(Vector3& position, float turnSpeed, float speed);
	void CheckPlayfieldSidesWarp();
	void CheckPlayfieldSidesWarp(float left, float right);
	void CheckPlayfieldHeightWarp(float top, float bottom);
	void AimAtTargetZ(Vector3& target, float facingAngle, float magnitute);
	void SetRotationZFromVectors(Vector3& target);
	void SetHeading(Vector3& waypoint, float rotationSpeed);
	void BeforeCalculate();
	void CalculateWorldSpace();
	void CalculateWorldVectors();
	void AfterCalculate();

private:
	float RadianSpin(float radian);
	float AddRotationVelAcc(float rotation, float rotationVelocity,
		float rotationAcceleration, float deltaTime);
};