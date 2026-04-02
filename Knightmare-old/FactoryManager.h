#pragma once
#include "EntityManager.h"
#include "Model3D.h"
#include "LineModel.h"

class FactoryManager
{
public:
	FactoryManager();
	virtual ~FactoryManager();

	void SetEntityManager(EntityManager& entityManager);

	bool Initialize();
	bool BeginRun();

	void Model3DFactory(Model3D* entity, Model& model, Color color,
	Vector3 position, Vector3 velocity);
	void LineModelFactory(LineModel* entity, LineModelPoints& model,
	Color color, Vector3 position, Vector3 velocity);

private:

	EntityManager* EM = nullptr;
};

