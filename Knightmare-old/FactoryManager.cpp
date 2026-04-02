#include "FactoryManager.h"

FactoryManager::FactoryManager()
{

}

FactoryManager::~FactoryManager()
{
}

void FactoryManager::SetEntityManager(EntityManager& entityManager)
{
	EM = &entityManager;
}

bool FactoryManager::Initialize()
{
	return false;
}

bool FactoryManager::BeginRun()
{

	return false;
}

void FactoryManager::Model3DFactory(Model3D* entity, Model& model, Color color, Vector3 position, Vector3 velocity)
{
	if (EM == nullptr)
	{
		TraceLog(LOG_INFO, "***************\n");
		TraceLog(LOG_ERROR,
			"*********  EM reference is missing!  ***************************\n");

		return;
	}

	EM->AddModel3D(entity, model);
	entity->ModelColor = color;
	entity->Initialize();
	entity->BeginRun();
	entity->Spawn(position);
	entity->Velocity = velocity;
}

void FactoryManager::LineModelFactory(LineModel* entity, LineModelPoints& model,
	Color color, Vector3 position, Vector3 velocity)
{
	if (EM == nullptr)
	{
		TraceLog(LOG_INFO, "***************\n");
		TraceLog(LOG_ERROR,
			"*********  EM reference is missing!  ***************************\n");

		return;
	}

	EM->AddLineModel(entity, model);
	entity->ModelColor = color;
	//entity->Cull = false;
	entity->Initialize();
	entity->BeginRun();
	entity->Spawn(position);
	entity->Velocity = velocity;
}