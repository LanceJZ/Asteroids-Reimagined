#pragma once
#include "Globals.h"
#include "ThePlayerControls.h"
#include "TheStarShot.h"

class TheAntiPlayer : public ThePlayerControls
{
public:
	TheAntiPlayer();
	virtual ~TheAntiPlayer();

	std::vector<TheStarShot*> StarShots;

	void SetStarShotModel(std::vector<Vector3> model);
	void SetStarShotSound(std::string sound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void SetPlayer(Entity *player);
	void Spawn(Vector3 position);
	void Destroy();

private:
	Sound StarShotSound = {};

	Entity* Player = {};

	std::vector<Vector3> StarShotModel = {};
};
