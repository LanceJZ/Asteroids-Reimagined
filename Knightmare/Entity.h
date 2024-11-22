#pragma once
#include "Common.h"
#include "ContentManager.h"

class Entity : public Common
{
public:
	bool EntityOnly = false;
	bool ShowCollision = false;
	bool HideCollision = false;
	bool IsChild = false;
	bool IsParent = false;
	bool IsConnectedChild = true;
	bool Enabled = true;
	bool IgnoreParentRotation = false;
	bool WasCulled = false;
	bool BeenHit = false;
	int ChildNumber = 0;
	float Scale = 1;
	float ModelScale = 1;
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
	float DeltaTime = 0;

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

	std::vector<Entity*> *Parents;
	std::string EntityName = {};

	Entity();
	virtual ~Entity();

	virtual bool Initialize(Utilities* utilities);
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float deltaTime);
	virtual void Draw3D();

	void X(float x);
	void Y(float y);
	void Z(float z);
	void SetScale(float scale);

	virtual void Spawn(Vector3 position);
	virtual void Hit();
	virtual void Destroy();

	bool GetBeenHit();
	bool CirclesIntersect(Vector3 targetPosition, float targetRadius);
	bool CirclesIntersect(Entity& target);
	bool CirclesIntersectBullet(Entity& target);
	bool ScreenEdgeBoundY();
	bool ScreenEdgeBoundY(float topOffset, float bottomOffset);
	bool ScreenEdgeBoundX();
	bool ScreenEdgeBoundX(float leftOffset, float rightOffset);
	bool IsOffScreen();
	bool IsOffScreenSide();
	bool IsOffScreenTopBottom();

	float X();
	float Y();
	float Z();
	float GetAngleFromVectorZ(Vector3 target);
	float GetAngleFromWorldVectorZ(Vector3 target);
	float GetAngleFromVectors(Vector3& target);

	Vector3 GetRandomVelocity(float magnitude);
	Vector3 GetVelocityFromAngleZ(float angle, float magnitude);
	Vector3 GetVelocityFromAngleZ(float magnitude);
	Vector3 GetAccelerationToMaxAtRotation(float accelerationAmount, float topSpeed);
	Vector3 GetWorldPosition();

	void SetRotationZTowardsTargetZ(Vector3& target, float magnitude);
	void SetAccelerationToMaxAtRotation(float accelerationAmount, float topSpeed);
	void SetAccelerationToZero(float decelerationAmount);
	void SetRotateVelocity(Vector3& position, float turnSpeed, float speed);
	void SetRotationZFromVector(Vector3& target);
	void SetHeading(Vector3& waypoint, float rotationSpeed);
	void SetAimAtTargetZ(Vector3& target, float facingAngle, float magnitute);
	void SetParent(Entity& parent);
	virtual bool SetCamera(Camera* camera);
	virtual void SetModel(Model &model, float scale);
	virtual void SetModel(Model &model);
	virtual void SetModelCopy(Model model, float scale);
	virtual void SetModelWithTexture(Model &model, Texture2D &texture);
	virtual LineModelPoints GetLineModel();
	virtual std::vector<Vector3> GetModel();
	virtual void SetModel(std::vector<Vector3> lines);
	virtual void SetModel(LineModelPoints lines);
	virtual void SetModel(LineModelPoints lines, float scale);
	virtual Model& Get3DModel();
	void RemoveParent(Entity* parent);
	void ClearParents();
	void CheckScreenEdge();
	void CheckScreenEdgeX();
	void CheckScreenEdgeY();
	void LeavePlay(float turnSpeed, float speed);
	void CheckPlayfieldSidesWarp();
	void CheckPlayfieldSidesWarp(float left, float right);
	void CheckPlayfieldHeightWarp(float top, float bottom);

protected:
	std::vector<Vector3> LinePoints;
	LineModelPoints Lines;

	Camera* TheCamera3D = {};
	Model TheModel = {};

	void BeforeCalculate();
	void CalculateWorldSpace();
	void CalculateWorldVectors();
	void AfterCalculate();

private:
	Ray TheRay = { 0 };
	//RayCollision TheRayCollision = { 0 };

	float RadianSpin(float radian);
	float AddRotationVelAcc(float rotation, float rotationVelocity,
		float rotationAcceleration, float deltaTime);

	void CalculateRadius();
};

