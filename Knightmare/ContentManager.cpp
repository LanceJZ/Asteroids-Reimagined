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

void ContentManager::ResetLineModels()
{
	LoadedLineModels.clear();
}

void ContentManager::SaveLineModel(std::string name, std::vector<Vector3> points)
{
	std::string modelPointsSTR = { };

	for (auto &point : points)
	{
		modelPointsSTR.append("(");
		modelPointsSTR.append(std::to_string(point.x));
		modelPointsSTR.append(", ");
		modelPointsSTR.append(std::to_string(point.y));
		modelPointsSTR.append(", ");
		modelPointsSTR.append(std::to_string(point.z));
		modelPointsSTR.append(")");
	}

	SaveFileText((char*)GetLineModelFileNameWithPath(name).c_str(), (char*)modelPointsSTR.c_str());
}

void ContentManager::SaveScene(std::string fileName, std::vector<Scene> sceneModels)
{
	std::string sceneSTR = { };

	for (auto& model : sceneModels)
	{
		sceneSTR.append("(");
		sceneSTR.append(std::to_string(model.Position.x));
		sceneSTR.append(", ");
		sceneSTR.append(std::to_string(model.Position.y));
		sceneSTR.append(", ");
		sceneSTR.append(std::to_string(model.Position.z));
		sceneSTR.append(")");
	}

	for (auto& model : sceneModels)
	{
		sceneSTR.append("Name;");
		sceneSTR.append(model.Name);
		sceneSTR.append(":");
	}

	SaveFileText(GetSceneFileNameWithPath(fileName).c_str(), (char*)sceneSTR.c_str());
}

std::vector<Scene> ContentManager::LoadScene(std::string fileName)
{
	//if (!FileExists(GetSceneFileNameWithPath(fileName).c_str()))
	//{
	//	TraceLog(LOG_INFO, "*****\n");
	//	TraceLog(LOG_ERROR, "***********************  Scene :%s missing. ***********************\n",
	//		(GetSceneFileNameWithPath));

	//	return {};
	//}

	std::vector<Scene> sceneModels = {};
	bool foundName = false;

	std::string sceneSTR = LoadFileText(GetSceneFileNameWithPath(fileName).c_str());

	std::vector<Vector3> positions = ConvertStringToPointsNew(sceneSTR);
	std::vector<std::string> names;
	std::string name;

	for (const auto& character : sceneSTR)
	{
		if (character == ':')
		{
			names.push_back(name);
			name.clear();
			foundName = false;
		}

		if (foundName)
		{
			name.append(1, character);
		}

		if (character == ';') foundName = true;
	}

	for (size_t i = 0; i < positions.size(); i++)  
	{
		sceneModels.push_back(Scene());
		sceneModels.back().Position = positions[i];
		sceneModels.back().Name = names[i];
	}

	return sceneModels;
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

std::string ContentManager::GetModelFileNameWithPath(std::string fileName)
{
	std::string path = "Models/";
	path.append(fileName);
	path.append(".obj");

	return path;
}

std::string ContentManager::GetLineModelFileNameWithPath(std::string fileName)
{
	std::string path = "Models/";
	path.append(fileName);
	path.append(".vec");

	return path;
}

std::string ContentManager::GetSceneFileNameWithPath(std::string fileName)
{
	std::string path = "Scenes/";
	path.append(fileName);
	path.append(".scn");

	return path;
}

Model& ContentManager::GetModel(size_t modelNumber)
{
	return LoadedModels[modelNumber];
}

std::vector<Vector3>& ContentManager::GetLineModel(size_t modelNumber)
{
	return LoadedLineModels[modelNumber];
}

Model& ContentManager::LoadAndGetModel(std::string modelFilename)
{
	return GetModel(LoadTheModel(modelFilename));
}

std::vector<Vector3>& ContentManager::LoadAndGetLineModel(std::string modelFileName)
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
	std::string namePNG = "Models/";;
	namePNG.append(modelFileName);
	namePNG.append(".png");

	std::string nameOBJ = GetLineModelFileNameWithPath(modelFileName);

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
	std::string nameWAV = "Sounds/";
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
std::vector<Vector3> ContentManager::LoadLineModel(std::string fileName)
{
	std::vector<Vector3> points;
	std::string nameVEC = GetLineModelFileNameWithPath(fileName);

	if (FileExists(nameVEC.c_str()))
	{
		std::string linesTemp = LoadFileText(nameVEC.c_str());

		if (linesTemp[1] == 88)
		{
			points = ConvertStringToPoints(linesTemp);
		}
		else
		{
			points = ConvertStringToPointsNew(linesTemp);
		}
	}
	else
	{
		TraceLog(LOG_INFO, "*****\n");
		TraceLog(LOG_ERROR, "***********************  Line Model :%s missing. ***********************\n",
			(nameVEC.c_str()));
		TraceLog(LOG_INFO, "******************* One Unit Square replaced missing model file. *******************\n");

		points.push_back({1,1,0});
		points.push_back({-1,1,0});
		points.push_back({-1,-1,0});
		points.push_back({1,-1,0});
		points.push_back({1,1,0});
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

	for (const auto& character : linesString)
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
			else if (character == 125 || character == 41)
			{
				line.z = stof(number);
				number = "";

				linesConverted.push_back(line);
			}
		}
	}

	return linesConverted;
}

std::vector<Vector3> ContentManager::ConvertStringToPointsNew(std::string linesString)
{
	std::string number;
	Vector3 line = { 0 };
	std::vector<Vector3> linesConverted;

	enum OnAxis
	{
		X,
		Y,
		Z
	};

	OnAxis onAxis = X;

	for (const auto& character : linesString)
	{
		if (character > 44 && character < 58)
		{
			number.append(1, character);
		}
		else
		{
			if (character == 40)
			{
				onAxis = X;
			}
			else if (character == 44 && onAxis == X)
			{
				onAxis = Y;
				line.x = stof(number);
				number = "";
			}
			else if (character == 44 && onAxis == Y)
			{
				onAxis = Z;
				line.y = stof(number);
				number = "";
			}
			else if (character == 41 && onAxis == Z)
			{
				line.z = stof(number);
				number = "";
				linesConverted.push_back(line);
			}
		}
	}

	return linesConverted;
}
