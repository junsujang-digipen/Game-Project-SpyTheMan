#pragma once
/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include "..\Engine\GameState.h"
#include "..\Engine\Engine.h"

constexpr unsigned NAMEMAX{ 20 };

enum class EditorState
{
	LevelSetting, // set chunk size, skill set
	RoomControl, // 
	ObjControl,
	DestroyObj, Count
};

class LevelEditor : public PM::GameState
{
public:
	LevelEditor();
	~LevelEditor();
	void Load() override;
	void Update(double dt) override;
	void Draw() override;
	void Unload() override;
	std::string GetName() override { return "LevelEditor"; }
private:
	void LoadIMGUI();
	void UpdateIMGUI(double dt);
	void UnLoadIMGUI();
	void SetChunkSize(glm::ivec2 size); //set Chunk size
	void LoadData(std::string name);
	void SaveData(std::string name);
	void IMGUILevelSettingUpdate();
	void IMGUIRoomControlUpdate();
	void IMGUIDestroyObjUpdate();
	void RoomControlUpdate();
	void ObjControlUpdate();
	
	void IMGUIObjControlUpdate();
	
	bool IsInsideChunk(glm::vec2 pos);
	bool IsInsideRoom(glm::vec2 pos);
	PM::RoomData* GetRoom(glm::vec2 pos);
	bool IsInsideRoom(glm::vec2 pos1, glm::vec2 pos2);
	void makeRoom(glm::vec2 pos1,glm::vec2 pos2);
	void destroyRoom(glm::vec2 pos);
	void DrawRoom(math::TransformMatrix& mat);
	glm::ivec2 GetCNum(glm::vec2 pos);
	void CheckSameName();
	
	void SetDoor(bool isLeft);
	void SetWall(bool isLeft);
	void DestroyObj(glm::vec2 pos);
	
	void ChangeState(EditorState state);
;	PM::LevelData* leveldata{nullptr};
	char* LevelName{ nullptr };
	char* tempObjName{ nullptr };
	char* tempTarget{ nullptr };
	EditorState state{EditorState::LevelSetting};
	bool loadbuttonOn{ false };
	bool PlayButtonOn{ false };
	glm::dvec2 bindingPos = {  };
	bool selectFirst{false};
	glm::ivec2 selectFirstInfo{ 0,0 };

	glm::vec2 tempObjectPos{}; 
	glm::vec4 tempSecondObjPos{};
	std::string tempObjectName{};
	std::string TargetName{};
	bool isLock{false};
	std::vector<char*> disguiseList;
	std::vector<char*> SideList;
	std::vector<EntityType> editAbleEntity;
	EntityType selectType{ EntityType::Count };

	int Lstate{0};
	int Rstate{0};

	 DisguiseList D_list{DisguiseList::Worker};
	 float onofftime{0};
	 bool isleft{false};
	 bool isMove = false;
	 double xsize{0};
};