/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <map>

#include "../../Game/Object/Room.h"
#include "../Game/SkillType.h"
#include "ChapterData.h"
namespace Json {
	class Value;
}

namespace PM
{
	class WorldData;
	class RoomData
	{
	public:
		~RoomData();
		std::string Name;
		glm::dvec2 position;
		glm::dvec2 size;
		std::string Left;
		std::string Right;
		Json::Value DataToValue();
	};
	class LevelData
	{
	public:
		LevelData(std::string lvName) : levelName(lvName) {}
		LevelData() = default;
		~LevelData() { Clear(); }
	private:
		std::string levelName{ "Error" };
		std::string ImagePath{"assets/Level/NoImage.png"};
		glm::ivec2 ChunkSize{ 5,5 };
		std::vector<RoomData*> roomData;
		std::vector<ObjData*> objData;

		int Chapter{ -1 };

		bool skillSet[static_cast<int>(SkillType::Null)]{false};
	public:
		void SetName(std::string name) { levelName = name; };
		void SetChunkSize(glm::ivec2 size) { ChunkSize = size; };
		void SetSkillset(SkillType skill, bool set) { skillSet[static_cast<int>(skill)] = set; }
		void SetChapter(int i) { Chapter = i; }
		glm::ivec2 GetChunkSize() const { return ChunkSize; }
		std::string GetName() const { return levelName; }
		bool GetSkillset(SkillType skill) { return skillSet[static_cast<int>(skill)]; }
		int GetChapter() { return Chapter; }
		std::vector<ObjData*>& GetobjData() { return objData; }
		std::vector<RoomData*>& GetRoomData() { return roomData; }

		void Clear();
	};

	class LevelIOManager
	{
		friend class LevelData;
		friend class ObjData;
	public:
		LevelIOManager() {}

		void WriteLevel(LevelData* Data); // for editor
		LevelData* ReadLevel(std::string levelName, std::string filePath); // for editor
		LevelData* ReadLevel();
		bool HaveLevel(std::string name, std::string filePath);
		void SetNextLevel(std::string name, std::string filePath);
		
		void SetChapter(int ChaterNumber, int LevelNumber);
		int GetChapterNumber() { return chapterNumber; }
		int GetLevelNumber() { return levelNumber; }
		void ResetAllChapter();
		void ClearStage(double cleartime);
		
		ChapterData* ReadChaterData(int ChaterNumber);
		void SaveChapterData(ChapterData* data);

	private:
		void WriteLevelJS(LevelData* Data);
		void WriteObjJS(LevelData* Data);
		
		LevelData* ReadLevelJS(std::string levelName , std::string filePath);
		std::vector<ObjData*> ReadObjJS(std::string levelName, std::vector<std::string> objname, std::string filePath);
		ObjData* ValueToObjData(Json::Value);
		
		private:
			std::string nextLevel;
			std::string nextFilePath;
			int chapterNumber{ -1 };
			int levelNumber{ -1 };
		//std::map<std::string, LevelData> levelInfos;
	};
}


