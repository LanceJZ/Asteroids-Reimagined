#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "raymath.h"

typedef struct LineModelPoints
{
	std::vector<Vector3> linePoints;
} LineModelPoints;

class ContentManager
{
public:
	ContentManager();
	virtual ~ContentManager();

	bool Initialize();
	bool BeginRun();

	size_t LoadTheModel(std::string modelFileName); //Without path or ext.
	size_t LoadTheLineModel(std::string modelFileName); //Without path or ext.
	size_t LoadTheSound(std::string soundFileName); //Without path or ext.
	size_t LoadTheTexture(std::string textureFileName); //Without path or ext.
	Model& GetModel(size_t modelNumber);
	Model& LoadAndGetModel(std::string modelFileName); //Without path or ext.
	LineModelPoints& GetLineModel(size_t modelNumber);
	LineModelPoints LoadAndGetLineModel(std::string modelFileName);
	Sound& GetSound(size_t soundNumber);
	Sound LoadAndGetSound(std::string soundFileName); //Without path or ext.
	Texture& GetTexture(size_t textureNumber);
	Texture LoadAndGetTexture(std::string textureFileName); //Without path or ext.

private:
	std::vector<Model> LoadedModels;
	std::vector<LineModelPoints> LoadedLineModels;
	std::vector<Sound> LoadedSounds;
	std::vector<Texture> LoadedTextures;
	LineModelPoints ThePoints;

	Model LoadModelWithTexture(std::string modelFileName);
	Model SetTextureToModel(Model model, Texture2D texture);
	Sound LoadSoundFile(std::string soundFileName);
	Texture LoadTextureFile(std::string textureFileName);

	LineModelPoints LoadLineModel(std::string fileName);
	std::vector<Vector3> ConvertStringToPoints(std::string fileName);
};

