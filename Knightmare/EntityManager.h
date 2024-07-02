#pragma once
#include <memory>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "Entity.h"
#include "OnScreenText.h"
#include "LineModel.h"
#include "Model3D.h"
#include "Timer.h"

#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
	// allocations to be of _CLIENT_BLOCK type
#else
	#define DBG_NEW new
#endif

class EntityManager
{
public:
	EntityManager();
	virtual ~EntityManager();

	bool Initialize();
	bool BeginRun();
	void SetCamera(Camera& camera);
	void SetUtilities(Utilities* utilities);

	void Input();
	void Update(float deltaTime);
	void Draw3D();
	void Draw2D();

	void ResetTimer(size_t id);
	void ResetTimer(size_t id, float amount);
	void SetTimer(size_t id, float amount);
	bool TimerElapsed(size_t id);

	size_t AddEntity(Entity* entity);
	size_t AddEntity();
	size_t AddLineModel(LineModel* model);
	size_t AddLineModel(LineModelPoints model);
	size_t AddLineModel(LineModel* model, LineModelPoints points);
	size_t AddLineModel();
	size_t AddModel3D(Model3D* model3D);
	size_t AddModel3D(Model3D* model3D, Model &model);
	size_t AddModel3D(Model3D* model3D, Model &model, float scale);
	size_t AddModel3D(Model &model);
	size_t AddModel3D(Model &model, float scale);
	size_t AddTimer();
	size_t AddTimer(float amount);
	size_t AddCommon(Common* common);
	size_t AddOnScreenText(OnScreenText* drawText);

	Entity* CreateEntity();
	LineModel* CreateLineModel();
	Model3D* CreateModel3D(Model model);

	std::vector<Entity*> Entities = {};
	std::vector<OnScreenText*> OnScreenTexts = {};
	std::vector<LineModel*> LineModels = {};
	std::vector<Model3D*> Model3Ds = {};
	std::vector<Common*> Commons = {};

private:
	Utilities* TheUtilities = {};
	Camera* TheCamera = {};

	std::vector<Timer*> Timers = {};
};

