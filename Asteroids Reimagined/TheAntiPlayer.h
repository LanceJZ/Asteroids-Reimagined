#pragma once
#include "Globals.h"
#include "LineModel.h"
#include "Enemy.h"
#include "Shot.h"
#include "TheStarShot.h"

class TheAntiPlayer : public Enemy
{
public:
	TheAntiPlayer();
	virtual ~TheAntiPlayer();

	std::vector<TheStarShot*> StarShots;

	bool Initialize();
	bool BeginRun();

	void SetPlayer(ThePlayer* player);
	void SetShotModel(std::vector<Vector3> model);

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:


};
