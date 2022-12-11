/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include <iostream>
//#include <fstream>
#include "LevelIOManager.h"

#include "../Engine.h"
#include "ObjectData.h"

#include "../GameObjectManager.h"
#include "../World.h"
#include <filesystem> // std::filesystem

#include "../../Game/Object/Clothes.h"
#include "json/json.h"
#include "../Declaration.h"
PM::RoomData::~RoomData()
{
}


Json::Value PM::RoomData::DataToValue()
{
	Json::Value root;
	root["NAME"] = Name;
	root["POSITION"].append(position.x);
	root["POSITION"].append(position.y);
	root["SIZE"].append(size.x);
	root["SIZE"].append(size.y);
	root["LEFT"] = Left;
	root["RIGHT"] = Right;
	return root;
}


bool PM::LevelIOManager::HaveLevel(std::string name, std::string filePath)
{
	std::ifstream inFile(filePath + name + "/" + name + ".json");
	if (inFile.is_open() == false)
	{
		Engine::GetLogger().LogError("error:can't read Level JS");
		return false;
	}
	return true;
}


void PM::LevelIOManager::SetNextLevel(std::string name, std::string filePath)
{
	nextLevel = name;
	nextFilePath = filePath;
}

void PM::LevelIOManager::SetChapter(int ChaterNumber, int LevelNumber)
{
	chapterNumber = ChaterNumber;
	levelNumber = LevelNumber;
}

void PM::LevelIOManager::ClearStage(double cleartime)
{
	if(chapterNumber < 0 || levelNumber < 0)
	{
		return;
	}
	PM::ChapterData* cdata = ReadChaterData(chapterNumber);
	if(cdata->levels.size() == levelNumber + 1)
	{
		cdata->isCleard = true;
	}


	
	if(cdata->levels[levelNumber]->IsCleard == false || cdata->levels[levelNumber]->ClearTime > cleartime)
	{
		cdata->levels[levelNumber]->ClearTime = cleartime;
	}

	cdata->levels[levelNumber]->IsCleard = true;
	SaveChapterData(cdata);
	delete cdata;
	
}

void PM::LevelIOManager::ResetAllChapter()
{
	for(int i = 0; i < ChapterNum; ++i)
	{
		PM::ChapterData* cdata = ReadChaterData(i);

			cdata->isCleard = false;
			for(auto& ldata  : cdata->levels)
			{
				ldata->IsCleard = false;
				ldata->ClearTime = 0.;
			}
		SaveChapterData(cdata);
		delete cdata;
	}
}


PM::LevelData* PM::LevelIOManager::ReadLevel(std::string levelName, std::string filePath)
{
	return ReadLevelJS(levelName, filePath);
}

PM::LevelData* PM::LevelIOManager::ReadLevel()
{
	return ReadLevel(nextLevel, nextFilePath);
}


PM::LevelData* PM::LevelIOManager::ReadLevelJS(std::string levelName, std::string filePath)
{
	std::ifstream inFile(filePath + levelName +"/" + levelName + ".json");
	if(inFile.is_open() == false)
	{
		Engine::GetLogger().LogError("error:can't read Level JS");
		return nullptr;
	}
	Json::Value root;
	inFile >> root;
	LevelData* LvD = new LevelData(levelName);
	std::vector<ObjData*>& obj = LvD->GetobjData(); // ref
	

	std::vector<RoomData*>& Rooms = LvD->GetRoomData();

	LvD->SetName(root["NAME"].asString());
	LvD->SetChunkSize({ root["CHUNKSIZE"][0].asInt(),root["CHUNKSIZE"][1].asInt() });

	for(auto rn : root["ROOM"])
	{
		RoomData* temp = new RoomData();
		temp->Name = rn["NAME"].asString();
		temp->position = { rn["POSITION"][0].asDouble(), rn["POSITION"][1].asDouble() };
		temp->size = { rn["SIZE"][0].asInt(), rn["SIZE"][1].asInt() };

			temp->Left = rn["LEFT"].asString();
			temp->Right = rn["RIGHT"].asString();
			Rooms.push_back(temp);
	}
	std::vector<std::string> objNames;
	for(auto name : root["OBJ"])
	{
		objNames.push_back(name.asString());
	}


	
	for (int i = 0; i < static_cast<int>(SkillType::Null); ++i)
	{
		LvD->SetSkillset(static_cast<SkillType>(i), root["SKILL"][i].asBool());
	}
	
	obj = ReadObjJS(levelName, objNames, filePath);
	
	return LvD;	
}

std::vector<PM::ObjData*> PM::LevelIOManager::ReadObjJS(std::string levelName, std::vector<std::string> objname, std::string filePath)
{
	std::vector<PM::ObjData*> objData;
	for(auto on : objname)
	{
		std::ifstream inFile(filePath + levelName + "/Object/" + on + ".json");
		if (!inFile.is_open())
		{
			Engine::GetLogger().LogError("Can't Find RoomJS File");
			continue;
		}
		Json::Value root;
		inFile >> root;
		objData.push_back(ValueToObjData(root));
	}
	return objData;
}


void PM::LevelIOManager::WriteLevel(LevelData* Data)
{

	std::string levelName = Data->GetName();
	std::filesystem::path savePath("Level/" + levelName);
	std::filesystem::path objPath("Level/" + levelName + "/Object");
	
	if(std::filesystem::exists(savePath))
	{
		std::filesystem::remove_all(savePath);
	}
	std::filesystem::create_directory(savePath);
	std::filesystem::create_directory(objPath);
	WriteLevelJS(Data);
	WriteObjJS(Data);
}

void PM::LevelIOManager::WriteLevelJS(LevelData* lvData)
{
	std::string levelName = lvData->GetName();

	std::string levelDataLocation = "Level/" + levelName;

		
		Json::Value level;
		level["NAME"] = levelName; // name
		glm::ivec2 csize = lvData->GetChunkSize();
		level["CHUNKSIZE"].append(csize.x); // chunksize[0]
		level["CHUNKSIZE"].append(csize.y); // chunksize[1]
	
		for (auto rn : lvData->GetRoomData())
		{
			level["ROOM"].append(rn->DataToValue());
		}

		for(auto name : lvData->GetobjData())
		{
			level["OBJ"].append(name->GetName());
		}

		for(int i = 0; i < static_cast<int>(SkillType::Null);++i)
		{
			level["SKILL"].append(lvData->GetSkillset(static_cast<SkillType>(i)));
		}
	
		std::ofstream outFile("Level/" + levelName + '/' + levelName + ".json", std::ios::out);
		outFile << level;
}

void PM::LevelIOManager::WriteObjJS(LevelData* Data)
{
	std::string levelName = Data->GetName();
		std::string objDataLocation = "Level/" + levelName + "/Object";
		for (auto oData : Data->GetobjData())
		{
			std::ofstream outFile(objDataLocation + "/" + oData->GetName() + ".json", std::ios::out);
			
			outFile << oData->DataToValue();
		}
}


void PM::LevelData::Clear()
{
	for (auto od : objData)
	{
		delete od;
	}
	for(auto rd : roomData)
	{
		delete rd;
	}
	roomData.clear();
	objData.clear();

}


PM::ObjData* PM::LevelIOManager::ValueToObjData(Json::Value value)
{
	EntityType type = static_cast<EntityType>(value["TYPE"].asInt());

	std::string name = value["NAME"].asString();
	glm::dvec2 pos = glm::dvec2({ value["POSITION"][0].asDouble() }, { value["POSITION"][1].asDouble() });

	switch (type)
	{
	case EntityType::Wall:
		{
		glm::vec2 siz = { value["SIZE"][0].asFloat(),value["SIZE"][1].asFloat() };
		return new WallData(name, pos, siz);
		}
	case EntityType::SideDoor:
	{
		bool lock = value["LOCK"].asBool();
		return new SideDoorData(name, pos, lock);
	}
		break;
	case EntityType::Vent_obj:
		{
			double xsize = value["XSIZE"].asDouble();
			return new VentData(name, pos, xsize);
		}
		break;
	case EntityType::Stair:
	{
		std::string target = value["TARGET"].asString();
		bool isdown = value["ISDOWN"].asBool();
		return new StairData(name, pos, target, isdown);
	}
		break;
	case EntityType::Key: 
	{
		std::string target = value["TARGET"].asString();
		return new KeyData(name, pos, target);
	}
		break;
	case EntityType::Clothes: 
	{
		DisguiseList d = static_cast<DisguiseList>(value["DISGUISE"].asInt());
		return new ClothesData(name, pos, d);
	}
		break;
	case EntityType::Lazer:
	{
		double time = value["TIME"].asDouble();
		Json::Value mov = value["MOVABLE"];
		glm::vec4 movable = { mov[0].asDouble(),mov[1].asDouble(),mov[2].asDouble(),mov[3].asDouble() };
		return new LazerData(name, pos, time, movable);
	}
		break;
	case EntityType::Worker_NPC: [[fallthrough]];
	case EntityType::Enemy_NPC:
		{
	
		Json::Value p1 = value["POINT1"];
		Json::Value p2 = value["POINT2"];
		glm::vec2 point1 = { p1[0].asFloat(),p1[1].asFloat() };
			glm::vec2 point2 = { p2[0].asFloat(),p2[1].asFloat() };
		return new NPCData(name, pos, type, point1, point2);
		}
		break;
	case EntityType::Switch: break;
	default:
		return new ObjData(name, pos, type);
		break;
	}
	return nullptr;

}



PM::ChapterData* PM::LevelIOManager::ReadChaterData(int ChaterNumber)
{
	std::string chapter = "Chapter" + to_string(ChaterNumber);
	std::ifstream inFile("assets/Level/" + chapter + "/" + chapter + ".json");
	if (inFile.is_open() == false)
	{
		Engine::GetLogger().LogError("error:can't read Chapter JS");
		return nullptr;
	}

	Json::Value root;
	inFile >> root;
	ChapterData* CD = new ChapterData(ChaterNumber);
	CD->isCleard = root["CLEAR"].asBool();
	for(auto s : root["STAGE"])
	{
		Stage* stage = new Stage();
		stage->levelName = s["NAME"].asString();
		stage->ClearTime = s["TIME"].asDouble();
		stage->IsCleard = s["CLEAR"].asBool();
		CD->levels.push_back(stage);
	}
	return CD;
}

void PM::LevelIOManager::SaveChapterData(ChapterData* data)
{
	std::string chapter = "Chapter" + to_string(data->chapterNumber);
	std::ofstream outFile("assets/Level/" + chapter + "/" + chapter + ".json", ios::out);
	Json::Value root;

	if (outFile.is_open() == false)
	{
		Engine::GetLogger().LogError("error:can't write Chapter JS");
		return;
	}

	root["CLEAR"] = data->isCleard;
	

	for(auto l : data->levels)
	{
		Json::Value root2;
		root2["NAME"] = l->levelName;
		root2["TIME"] = l->ClearTime;
		root2["CLEAR"] = l->IsCleard;
		root["STAGE"].append(root2);
	}

	outFile << root;
}
