#include "EntityManager.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	for (auto model : LineModels)
	{
		delete(model);
	}

	for (auto model : Model3Ds)
	{
		delete(model);
	}

	for (auto entity : Entities)
	{
		delete(entity);
	}

	for (auto timer : Timers)
	{
		delete(timer);
	}

	for (auto common : Commons)
	{
		delete(common);
	}

	Entities.clear();
	LineModels.clear();
	Model3Ds.clear();
	Timers.clear();
	Commons.clear();
}

bool EntityManager::Initialize()
{
	for (auto common : Commons)
	{
		common->Initialize(TheUtilities);
	}

	for (auto model3D : Model3Ds)
	{
		model3D->Initialize(TheUtilities);
	}

	for (auto lineModel : LineModels)
	{
		lineModel->Initialize(TheUtilities);
	}
	return true;
}

bool EntityManager::BeginRun()
{
	for (auto model3D : Model3Ds)
	{
		model3D->BeginRun();
	}

	for (auto lineModel : LineModels)
	{
		lineModel->BeginRun();
	}

	for (auto common : Commons)
	{
		common->BeginRun();
	}

	return true;
}

void EntityManager::SetCamera(Camera& camera)
{
	TheCamera = &camera;

	for (auto model3D : Model3Ds)
	{
		model3D->SetCamera(TheCamera);
	}
}

void EntityManager::SetUtilities(Utilities* utilities)
{
	TheUtilities = utilities;
}

void EntityManager::Input()
{
	for (size_t i = 0; i < LineModels.size(); i++)
	{
		LineModels[i]->Input();
	}

	for (size_t i = 0; i < Model3Ds.size(); i++)
	{
		Model3Ds[i]->Input();
	}
}

void EntityManager::Update(float deltaTime)
{
	for (auto entity : Entities)
	{
		if (entity->Enabled) entity->Update(deltaTime);
	}

	for (auto lm : LineModels)
	{
		if (lm->Enabled) lm->Update(deltaTime);
	}

	for (int i = 0; i < Model3Ds.size(); i++)
	{
		if (Model3Ds[i]->Enabled) Model3Ds[i]->Update(deltaTime);
	}

	for (auto timer : Timers)
	{
		timer->Update(deltaTime);
	}

	for (auto common : Commons)
	{
		common->Update();
	}
}

void EntityManager::Draw3D()
{
	for (auto entity : Entities)
	{
		if (entity->Enabled) entity->Draw3D();
	}

	for (auto lineModel : LineModels)
	{
		if (lineModel->Enabled)	lineModel->Draw3D();
	}

	for (auto model3D : Model3Ds)
	{
		if (model3D->Enabled) model3D->Draw3D();
	}
}

void EntityManager::Draw2D()
{
	for (auto common : Commons)
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

size_t EntityManager::AddLineModel(LineModel* model)
{
	size_t number = LineModels.size();

	LineModels.push_back(model);
	LineModels[number]->Initialize(TheUtilities);

	return number;
}

size_t EntityManager::AddLineModel(LineModelPoints model)
{
	size_t number = LineModels.size();

	LineModels.push_back(DBG_NEW LineModel());
	LineModels[number]->SetModel(model);
	LineModels[number]->Initialize(TheUtilities);

	return number;
}

size_t EntityManager::AddLineModel(LineModel* model, LineModelPoints modelPoints)
{
	if (model == nullptr)
	{
		return -1;
	}

	size_t number = AddLineModel(model);

	LineModels[number]->SetModel(modelPoints);

	return number;
}

size_t EntityManager::AddLineModel()
{
	size_t number = LineModels.size();

	LineModels.push_back(DBG_NEW LineModel());
	LineModels[number]->Initialize(TheUtilities);

	return number;
}

size_t EntityManager::AddModel3D(Model3D* model3D)
{
	size_t modelNumber = Model3Ds.size();

	Model3Ds.push_back(model3D);

	return modelNumber;
}

size_t EntityManager::AddModel3D(Model3D* model3D, Model &model)
{
	if (model3D == nullptr)
	{
		return -1;
	}

	return AddModel3D(model3D, model, 1.0f);
}

size_t EntityManager::AddModel3D(Model3D* model3D, Model &model, float scale)
{
	if (model3D == nullptr)
	{
		return -1;
	}

	size_t modelNumber = AddModel3D(model3D);
	Model3Ds[modelNumber]->SetModel(model, scale);
	Model3Ds[modelNumber]->SetCamera(TheCamera);

	return modelNumber;
}

size_t EntityManager::AddModel3D(Model &model)
{
	size_t modelNumber = Model3Ds.size();
	Model3Ds.push_back(DBG_NEW Model3D());
	Model3Ds[modelNumber]->SetModel(model, 1.0f);
	Model3Ds[modelNumber]->Initialize(TheUtilities);
	Model3Ds[modelNumber]->SetCamera(TheCamera);

	return modelNumber;
}

size_t EntityManager::AddModel3D(Model &model, float scale)
{
	size_t modelNumber = AddModel3D(model);
	Model3Ds[modelNumber]->SetCamera(TheCamera);
	Model3Ds[modelNumber]->Scale = scale;

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

Entity* EntityManager::CreateEntity()
{
	Entity* newEntity = DBG_NEW Entity();
	Entities.push_back(newEntity);
	newEntity->Initialize(TheUtilities);
	newEntity->BeginRun();

	return newEntity;
}

LineModel* EntityManager::CreateLineModel()
{
	LineModel* newLineModel = DBG_NEW LineModel();
	LineModels.push_back(newLineModel);
	newLineModel->Initialize(TheUtilities);
	newLineModel->BeginRun();

	return newLineModel;
}

Model3D* EntityManager::CreateModel3D(Model model)
{
	Model3D* newModel3D = DBG_NEW Model3D();
	Model3Ds.push_back(newModel3D);
	newModel3D->SetModel(model, 1.0f);
	newModel3D->Initialize(TheUtilities);
	newModel3D->SetCamera(TheCamera);

	return newModel3D;
}