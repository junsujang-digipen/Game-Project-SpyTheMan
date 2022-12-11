/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
Creation date: 10/12/2021
-----------------------------------------------------------------*/
#pragma once
#include <gsl/gsl>
#include <vector>
#include <list>
#include <glm/vec2.hpp>


#include "Declaration.h"
#include "Input.h"
#include "Component.h"

namespace PM {
	class World;
}

namespace math {
	class TransformMatrix;
}

namespace PM
{
	class Room;
	class GameObject;
	
	struct OldObjData
	{
		GameObject* object;
		glm::vec2 oldpos;
		int oldlayer;
	};

	struct Chunk
	{
		friend PM::World;
		Chunk() = default;
		Chunk(glm::ivec2 chunkNumber);
		glm::vec2 position{ -1,-1 };
		glm::vec2 size{ -1,-1 };
		glm::vec2 chunkNumber{ -1,-1 };
		Room* roomPtr{ nullptr };
		std::list<GameObject*>& GetObjects() { return objs; }
		glm::vec2 GetPosition() const { return position; }
		glm::vec2 Getsize() const { return size; }
		glm::vec2 GetChunkNumber() const { return chunkNumber; }
	private:
		void SetChunkNumber(glm::ivec2 chunkNumber);
		void draw(math::TransformMatrix& cameraMatrix) const;
		std::list<GameObject*> objs;
	};
	class World : public Component
	{
	public:

		World(glm::ivec2 world_Chunk_size);
		~World();

		void Add(GameObject* obj, int layer = 1);
		void AddRoom(Room* room, int layer = 1);
		void Update(double dt) override;
		void DrawAll(math::TransformMatrix& cameraMatrix) const;
		void DetectNPCEffect(math::TransformMatrix& cameraMatrix) const;
		void CollideTest();
		

		void SetRoomVision(GameObject* obj);
		
		/// Get Chunk Number
		int GetChunkNumber(glm::vec2);
		int GetChunkNumber(GameObject*);
		std::vector<int> GetAroundChunkNumber(int num);
		std::vector<int> GetAroundChunkNumber(glm::vec2 pos); 
		std::vector<int> GetAroundChunkNumber(GameObject* obj);

		std::vector<int> GetFrontChunkNumber(GameObject* obj);
		std::vector<int> GetChunkNumber(Room*);
		int GetFloorNumber(GameObject* obj);
		int GetFloorNumber(glm::vec2 pos);
		int GetFloorNumber(Chunk* chunk);
		int GetFloorNumber(int index);
		//Get Chunk
		Chunk& GetChunk(GameObject* obj);
		Chunk& GetChunk(int layer, glm::vec2 pos);
		std::vector<Chunk*> GetAllFloorChunk(GameObject*);
		std::vector<Chunk*> GetAllFloorChunk(Chunk*, int layer);
		std::vector<Chunk*> GetAllFloorChunk(Room*);
		std::vector<Chunk*> GetAllFloorChunk(glm::vec2 pos, int layer);
		std::vector<Chunk*> GetAllFloorChunk(int floorNumber, int layer);
		std::vector<Chunk*> GetChunk(Room*);
		std::vector<GameObject*> GetNearEnemy(unsigned int Left = 0, unsigned int Right = 0, unsigned int Up = 0, unsigned int Down = 0);
		std::vector<GameObject*> GetNearNPC(unsigned int Left = 0, unsigned int Right = 0, unsigned int Up = 0, unsigned int Down = 0, PM::GameObject* obj = nullptr);
		std::vector<GameObject*> GetAllNPC();
		glm::vec2& GetFloorSize(GameObject*);
		Room* GetRoom(GameObject*);
		Room* GetRoom(int layer, int chunk);
		GameObject* GetObject(std::string name);
		
		GameObject* GetPlayerPtr() { return playerPtr; }
		glm::vec2 GetChunkSize() { return worldChunkSize; }
		int GetChunkSizeMax() { return worldChunkSizeMax; }
		Chunk& GetChunk(int layer, int chunkNum) { return worldChunk[layer][chunkNum]; }
		std::vector<Chunk>& GetWorldChunk(int layer) { return worldChunk[layer]; }
		std::vector<std::vector<Chunk>>& GetWorldChunk() { return worldChunk; }
		std::vector<Room*>& Rooms() { return rooms; }
		std::list<GameObject*>& GetObjects() { return gameObjects; }
		void SetStopUpdate(bool stop) { stopUpdate = stop; }
		int GetCurrLayer() { return currentLayer; }
		bool GetCanUseButton() const { return canUseButton; }
		void SetCanUseButton(bool set) { canUseButton = set; }
		double GetdtMagnification() { return dtMagnification; }
		void SetdtMagnification(double dtm) { dtMagnification = dtm; }
		void SetIsClear(bool set) { isClear = set; }
		bool GetIsClear() const { return isClear; }
		void SetIsDetectNPC(bool set) { isDetectNPC = set; }
		bool GetIsDetectNPC() const { return isDetectNPC; }
	private:
		//bool CanSetRoom(Room* room);

		std::vector<gsl::owner<Room*>> rooms;

		std::map<int, glm::vec2> floorsize;

		std::vector<Room*> drawRoomList;
		std::list<gsl::owner<GameObject*>> gameObjects;
		std::vector<std::vector<Chunk>> worldChunk;
		GameObject* playerPtr = nullptr;
		//Entity* mousePtr = nullptr;
		const glm::ivec2 worldChunkSize;
		const int worldChunkSizeMax;
		double dtMagnification = 1;
		int currentLayer = 1;
		bool drawChunk = false;
		bool drawAllObject = false;
		bool stopUpdate = false;
		bool canUseButton = true;
		bool isClear = false;
		bool isDetectNPC = false;

		float detectEffectSize = 0;
		
		PM::InputKey layerKey;
		PM::InputKey drawChunkKey;
		PM::InputKey drawAllObjectKey;
	};
}