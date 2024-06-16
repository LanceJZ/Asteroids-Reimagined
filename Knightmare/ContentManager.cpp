#include "ContentManager.h"

ContentManager::ContentManager()
{

}

ContentManager::~ContentManager()
{
	for (int i = 0; i < LoadedModels.size(); i++)
	{
		UnloadModel(LoadedModels[i]);
	}

	for (int i = 0; i < LoadedSounds.size(); i++)
	{
		UnloadSound(LoadedSounds[i]);
	}

	for (int i = 0; i < LoadedTextures.size(); i++)
	{
		UnloadTexture(LoadedTextures[i]);
	}

	LoadedModels.clear();
	LoadedLineModels.clear();
	LoadedSounds.clear();
	LoadedTextures.clear();
}

bool ContentManager::Initialize()
{

	return true;
}

bool ContentManager::BeginRun()
{

	return true;
}

size_t ContentManager::LoadTheModel(std::string modelFileName)
{
	LoadedModels.push_back(LoadModelWithTexture(modelFileName));

	return LoadedModels.size() - 1;
}

size_t ContentManager::LoadTheLineModel(std::string modelFileName)
{
	LoadedLineModels.push_back(LoadLineModel(modelFileName));

	return LoadedLineModels.size() - 1;
}

size_t ContentManager::LoadTheSound(std::string soundFileName)
{
	LoadedSounds.push_back(LoadSoundFile(soundFileName));

	return LoadedSounds.size() - 1;
}

size_t ContentManager::LoadTheTexture(std::string textureFileName)
{
	LoadedTextures.push_back(LoadTextureFile(textureFileName));

	return LoadedTextures.size() - 1;
}

Model& ContentManager::GetModel(size_t modelNumber)
{
	return LoadedModels[modelNumber];
}

Model& ContentManager::LoadAndGetModel(std::string modelFilename)
{
	return GetModel(LoadTheModel(modelFilename));
}

LineModelPoints& ContentManager::GetLineModel(size_t modelNumber)
{
	return LoadedLineModels[modelNumber];
}

LineModelPoints ContentManager::LoadAndGetLineModel(std::string modelFileName)
{
	return GetLineModel(LoadTheLineModel(modelFileName));
}

Sound& ContentManager::GetSound(size_t soundNumber)
{
	return LoadedSounds[soundNumber];
}

Sound ContentManager::LoadAndGetSound(std::string soundFileName)
{
	return GetSound(LoadTheSound(soundFileName));
}

Texture& ContentManager::GetTexture(size_t textureNumber)
{
	return LoadedTextures[textureNumber];
}

Texture ContentManager::LoadAndGetTexture(std::string textureFileName)
{
	return GetTexture(LoadTheTexture(textureFileName));
}
//Load OBJ model file only with texture/material in same folder no path or ext.
Model ContentManager::LoadModelWithTexture(std::string modelFileName)
{
	std::string path = "Models/";

	std::string namePNG = path;
	namePNG.append(modelFileName);
	namePNG.append(".png");

	std::string nameOBJ = path;
	nameOBJ.append(modelFileName);
	nameOBJ.append(".obj");
	Image image = { 0 };
	Model loadModel = { 0 };

	if (FileExists(nameOBJ.c_str()) &&
		FileExists(namePNG.c_str()))
	{
		loadModel = SetTextureToModel(LoadModel((nameOBJ.c_str())),
			LoadTexture(namePNG.c_str()));
	}
	else
	{
		TraceLog(LOG_INFO, "*****\n");
		TraceLog(LOG_ERROR, "***********************  Model  :%s missing. ***********************\n",
			(nameOBJ.c_str()));
		TraceLog(LOG_ERROR, "***********************  Texture  :%s missing. ***********************\n",
			(namePNG.c_str()));
	}

	return loadModel;
}

Model ContentManager::SetTextureToModel(Model model, Texture2D texture)
{
	//if (IsTextureReady(texture))
	//{
	//	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	//}

	return model;
}

Sound ContentManager::LoadSoundFile(std::string soundFileName)
{
	std::string path = "Sounds/";
	std::string nameWAV = path;
	nameWAV.append(soundFileName);
	nameWAV.append(".wav");

	if (!FileExists(nameWAV.c_str()))
	{
		TraceLog(LOG_INFO, "*****\n");
		TraceLog(LOG_ERROR, "***********************  Sound :%s missing. ***********************\n",
			(nameWAV.c_str()));
	}

	return LoadSound(nameWAV.c_str());
}
//Load PNG file only, without path or ext.
Texture ContentManager::LoadTextureFile(std::string textureFileName)
{
	std::string path = "Textures/";
	std::string namePNG = path;
	namePNG.append(textureFileName);
	namePNG.append(".png");

	if (!FileExists(namePNG.c_str()))
	{
		TraceLog(LOG_INFO, "*****\n");
		TraceLog(LOG_ERROR, "*********************** Texture :%s missing. ***********************\n",
			(namePNG.c_str()));
	}

	return LoadTexture(namePNG.c_str());
}
//Load VEC file only, without path or ext.
LineModelPoints ContentManager::LoadLineModel(std::string fileName)
{
	LineModelPoints points;

	std::string path = "Models/";
	std::string nameVEC = path;
	nameVEC.append(fileName);
	nameVEC.append(".vec");

	if (FileExists(nameVEC.c_str()))
	{
		std::string linesTemp = LoadFileText(nameVEC.c_str());
		points.linePoints = ConvertStringToPoints(linesTemp);
	}
	else
	{
		TraceLog(LOG_INFO, "*****\n");
		TraceLog(LOG_ERROR, "***********************  Line Model :%s missing. ***********************\n",
			(nameVEC.c_str()));
		TraceLog(LOG_INFO, "******************* One Unit Square replaced missing model file. *******************\n");

		points.linePoints.push_back({1,1,0});
		points.linePoints.push_back({-1,1,0});
		points.linePoints.push_back({-1,-1,0});
		points.linePoints.push_back({1,-1,0});
		points.linePoints.push_back({1,1,0});
	}

	return points;
}

std::vector<Vector3>  ContentManager::ConvertStringToPoints(std::string linesString)
{
	bool isNumber = false;
	float x = 0;
	float y = 0;
	float z = 0;
	std::string number;
	std::string onAxis;
	Vector3 line = { 0 };
	std::vector<Vector3> linesConverted;

	for (auto character : linesString)
	{
		if (character > 44 && character < 58)
		{
			number.append(1, character);
		}
		else
		{
			if (character == 88)
			{
				onAxis = "X";
			}
			else if (character == 89)
			{
				onAxis = "Y";

				line.x = stof(number);
				number = "";
			}
			else if (character == 90)
			{
				onAxis = "Z";

				line.y = stof(number);
				number = "";
			}
			else if (character == 125)
			{
				line.z = stof(number);
				number = "";

				linesConverted.push_back(line);
			}
		}
	}

	return linesConverted;
}
