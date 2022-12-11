/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Room.h
Project: Spy the Man
Author: Seongwon Jang
Creation date: 10/12/2021
-----------------------------------------------------------------*/
#pragma once
#include <list>

#include "SideDoor.h"
#include "../Engine/Texture.h"
#include "../Engine/Declaration.h"
#include<glm/glm.hpp>

namespace PM
{
	class Room : public GameObject
	{
	public:
		Room(std::string name, glm::vec2 pos, glm::vec2 size, GameObject* left, GameObject* right, std::string image = "assets/GameObject/proto/PNG/Green/texture_02.png", int security = 0);
		~Room();
		class RoomData
		{
		public:
			std::string roomName;
			glm::dvec2 position;
			glm::dvec2 size;
		};

	
		void Update(double dt) override;
		//void Draw(math::TransformMatrix displayMatrix) override;
		void DrawRoomImage(math::TransformMatrix& displayMatrix);
		
		void DrawEnemy(math::TransformMatrix& displayMatrix);
		void DrawBlackRect(math::TransformMatrix& displayMatrix);
		void SetValid(bool valid);
		bool GetValid() const { return Valid; }
		bool GetWasValid() const { return wasValid; }
		int GetSecurityLevel() const { return securityLevel; }
		GameObject* GetLeftObj() const { return Left; }
		GameObject* GetRightObj() const { return Right; }
		Room* GetLeftRoom();
		Room* GetRightRoom();


		EntityType GetEntityType() const { return EntityType::Room; }
		std::string GetEntityTypeName() const { return "Room"; }
		
		const std::vector<GameObject*> Objects();

		static constexpr  glm::vec3 BUILDINGCOLOR = {100,100,100};
	private:
		//std::list<GameObject*> gameObjects;
		PM::Texture* roomImage;
		GameObject* Left = nullptr;
		GameObject* Right = nullptr;
		float transparency = 0;
		int securityLevel{ 0 };
		bool Valid{ false };
		bool wasValid{false};
		std::list<Entity*>enemy;
	};

}
