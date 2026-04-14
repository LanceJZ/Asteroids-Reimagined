#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "raymath.h"

struct Scene
{
	std::string Name = {};
	Vector3 Position = {};
};

class ContentManager
{
public:
	ContentManager();
	virtual ~ContentManager();

	bool Initialize();
	bool BeginRun();

	void ResetLineModels();
	void SaveLineModel(std::string fileName, std::vector<Vector3> points);
	void SaveScene(std::string fileName, std::vector<Scene> sceneModels);
	std::vector<Scene> LoadScene(std::string fileName);

	size_t LoadTheModel(std::string modelFileName); //Without path or ext.
	size_t LoadTheLineModel(std::string modelFileName); //Without path or ext.
	size_t LoadTheSound(std::string soundFileName); //Without path or ext.
	size_t LoadTheTexture(std::string textureFileName); //Without path or ext.
	std::string GetModelFileNameWithPath(std::string fileName); //Just the name without path or ext, return name with path and ext.
	std::string GetLineModelFileNameWithPath(std::string fileName); //Same as model.
	std::string GetSceneFileNameWithPath(std::string fileName); //Same as model.
	Model& GetModel(size_t modelNumber);
	std::vector<Vector3>& GetLineModel(size_t modelNumber);
	Model& LoadAndGetModel(std::string modelFileName); //Without path or ext.
	std::vector<Vector3>& LoadAndGetLineModel(std::string modelFileName);
	Sound& GetSound(size_t soundNumber);
	Sound LoadAndGetSound(std::string soundFileName); //Without path or ext.
	Texture& GetTexture(size_t textureNumber);
	Texture LoadAndGetTexture(std::string textureFileName); //Without path or ext.
	std::vector<Vector3> ConvertStringToPointsNew(std::string linesString);

private:
	std::vector<Model> LoadedModels = {};
	std::vector<std::vector<Vector3>> LoadedLineModels = {};
	std::vector<Sound> LoadedSounds = {};
	std::vector<Texture> LoadedTextures = {};

	Model LoadModelWithTexture(std::string modelFileName);
	Model SetTextureToModel(Model model, Texture2D texture);
	Sound LoadSoundFile(std::string soundFileName);
	Texture LoadTextureFile(std::string textureFileName);

	std::vector<Vector3> LoadLineModel(std::string fileName);
	std::vector<Vector3> ConvertStringToPoints(std::string linesString);

};

