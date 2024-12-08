#include "EntityManager.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	for (const auto& entity : Entities)
	{
		delete(entity);
	}

	for (const auto& timer : Timers)
	{
		delete(timer);
	}

	for (const auto& common : Commons)
	{
		delete(common);
	}

	Entities.clear();
	Timers.clear();
	Commons.clear();
}

bool EntityManager::Initialize()
{
	for (const auto& common : Commons)
	{
		common->Initialize(TheUtilities);
	}

	for (const auto& Entity : Entities)
	{
		Entity->Initialize(TheUtilities);
	}

	return true;
}

bool EntityManager::BeginRun()
{
	for (const auto& common : Commons)
	{
		common->BeginRun();
	}

	for (const auto& entity : Entities)
	{
		entity->BeginRun();
	}

	return true;
}

void EntityManager::SetCamera(Camera& camera)
{
	TheCamera = &camera;

	for (const auto& entity : Entities)
	{
		entity->SetCamera(TheCamera);
	}
}

void EntityManager::SetUtilities(Utilities* utilities)
{
	TheUtilities = utilities;
}

void EntityManager::Input()
{
	for (const auto& common : Commons)
	{
		common->Input();
	}

	for (int i = 0; i < Entities.size(); i++)
	{
		if (Entities.at(i)->Enabled) Entities.at(i)->Input();
	}
}

void EntityManager::Update(float deltaTime)
{
	float halfDeltaTime = deltaTime * 0.5f;

	for (int i = 0; i < Entities.size(); i++)
	{
		if (Entities.at(i)->Enabled) Entities.at(i)->Update(halfDeltaTime);
	}

	for (int i = 0; i < Entities.size(); i++)
	{
		if (Entities.at(i)->Enabled) Entities.at(i)->Update(halfDeltaTime);
	}

	for (const auto& timer : Timers)
	{
		timer->Update(deltaTime);
	}

	for (const auto& common : Commons)
	{
		common->Update();
	}
}

void EntityManager::FixedUpdate(float deltaTime)
{
	for (const auto& timer : Timers)
	{
		timer->FixedUpdate(deltaTime);
	}

	for (const auto& common : Commons)
	{
		common->FixedUpdate();
	}

	for (int i = 0; i < Entities.size(); i++)
	{
		if (Entities.at(i)->Enabled) Entities.at(i)->FixedUpdate(deltaTime);
	}
}

void EntityManager::Draw3D()
{
	for (int i = 0; i < Entities.size(); i++)
	{
		if (Entities.at(i)->Enabled) Entities.at(i)->Draw3D();
	}
}

void EntityManager::Draw2D()
{
	for (const auto& common : Commons)
	{
		common->Draw2D();
	}
}

void EntityManager::ResetTimer(size_t id)
{
	Timers[id]->Reset();
}

void EntityManager::ResetTimer(size_t id, float amount)
{
	Timers[id]->Reset(amount);
}

void EntityManager::SetTimer(size_t id, float amount)
{
	Timers[id]->Set(amount);
}

bool EntityManager::TimerElapsed(size_t id)
{
	return Timers[id]->Elapsed();
}

float EntityManager::GetTimerAmount(size_t id)
{
	return Timers[id]->GetAmount();
}

float EntityManager::GetTimerAmountElapsed(size_t id)
{
	return Timers[id]->GetAmountElapsed();
}

size_t EntityManager::AddEntity(Entity* entity)
{
	size_t entityNumber = Entities.size();
	Entities.push_back(entity);
	Entities[entityNumber]->Initialize(TheUtilities);
	Entities[entityNumber]->EntityOnly = true;
	Entities[entityNumber]->BeginRun();

	return entityNumber;
}
//TODO: Change to use CreateEntity(* entity) private function for shared code.
size_t EntityManager::AddEntity()
{
	size_t entityNumber = Entities.size();
	Entity* newEntity = DBG_NEW Entity();
	Entities.push_back(newEntity);
	Entities[entityNumber]->Initialize(TheUtilities);
	Entities[entityNumber]->EntityOnly = true;
	Entities[entityNumber]->BeginRun();

	return entityNumber;
}

size_t EntityManager::AddLineModel(Entity* model)
{
	size_t number = Entities.size();

	Entities.push_back(model);
	Entities[number]->Initialize(TheUtilities);

	return number;
}

size_t EntityManager::AddLineModel(LineModelPoints model)
{
	size_t number = Entities.size();

	Entities.push_back(DBG_NEW Entity());
	Entities[number]->SetModel(model);
	Entities[number]->Initialize(TheUtilities);

	return number;
}

size_t EntityManager::AddLineModel(Entity* model, LineModelPoints modelPoints)
{
	if (model == nullptr)
	{
		return -1;
	}

	size_t number = AddLineModel(model);

	Entities[number]->SetModel(modelPoints);

	return number;
}

size_t EntityManager::AddLineModel()
{
	size_t number = Entities.size();

	Entities.push_back(DBG_NEW Entity());
	Entities[number]->Initialize(TheUtilities);

	return number;
}

size_t EntityManager::AddModel3D(Entity* model3D)
{
	size_t modelNumber = Entities.size();

	Entities.push_back(model3D);
	Entities.at(modelNumber)->Initialize(TheUtilities);
	Entities.at(modelNumber)->SetCamera(TheCamera);

	return modelNumber;
}

size_t EntityManager::AddModel3D(Entity* model3D, Model &model)
{
	if (model3D == nullptr)
	{
		return -1;
	}

	return AddModel3D(model3D, model, 1.0f);
}

size_t EntityManager::AddModel3D(Entity* model3D, Model &model, float scale)
{
	if (model3D == nullptr)
	{
		return -1;
	}

	size_t modelNumber = AddModel3D(model3D);
	Entities[modelNumber]->SetModel(model, scale);
	Entities[modelNumber]->SetCamera(TheCamera);
	Entities[modelNumber]->Initialize(TheUtilities);
	Entities[modelNumber]->BeginRun();

	return modelNumber;
}

size_t EntityManager::AddModel3D(Model &model)
{
	size_t modelNumber = Entities.size();
	Entities.push_back(DBG_NEW Entity());
	Entities[modelNumber]->SetModel(model, 1.0f);

	return modelNumber;
}

size_t EntityManager::AddModel3D(Model &model, float scale)
{
	size_t modelNumber = AddModel3D(model);
	Entities[modelNumber]->Scale = scale;

	return modelNumber;
}

size_t EntityManager::AddTimer()
{
	size_t timerNumber = Timers.size();
	Timers.push_back(DBG_NEW Timer());

	return timerNumber;
}

size_t EntityManager::AddTimer(float amount)
{
	size_t number = AddTimer();
	Timers[number]->Reset(amount);

	return number;
}

size_t EntityManager::AddCommon(Common* common)
{
	size_t commonNumber = Commons.size();
	Commons.push_back(common);
	Commons[commonNumber]->Initialize(TheUtilities);

	return commonNumber;
}

size_t EntityManager::AddOnScreenText(Common* onScreenText)
{
	size_t number = Commons.size();
	Commons.push_back(onScreenText);
	Commons[number]->Initialize(TheUtilities);

	return number;
}

Entity* EntityManager::CreateEntity()
{
	Entity* newEntity = DBG_NEW Entity();
	Entities.push_back(newEntity);
	newEntity->Initialize(TheUtilities);
	newEntity->BeginRun();

	return newEntity;
}