/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include <glm/vec2.hpp>
#include <json/value.h>


#include "../GameObject.h"

namespace PM
{
	class ObjData
	{
	public:
		friend class ComponentData;
		
		ObjData(std::string objName);
		ObjData(std::string objName, glm::vec2 position, EntityType type);
		virtual ~ObjData() {}
		
		virtual Json::Value DataToValue();
		virtual PM::GameObject* DataToObj();
		EntityType GetEntityType() const { return GOT; }
		virtual void Draw(math::TransformMatrix& displayMatrix);
		
		std::string GetName() const { return objName; }
		glm::vec2 GetPosition() const { return position; }
		
	protected:
		EntityType GOT = EntityType::Empty;
		std::string objName{ "Empty" };
		glm::vec2 position{};
	};

	class StairData : public ObjData
	{
	public:
		StairData(std::string objName, glm::vec2 pos, std::string target, bool isdown)
			:ObjData(objName, pos, EntityType::Stair), Target(target),isDown(isdown) {}
		PM::GameObject* DataToObj() override;
		Json::Value DataToValue() override;
	protected:
		std::string Target{ "Empty" };
		bool isDown{ false };
	};


	class SideDoorData : public ObjData
	{
	public:
		SideDoorData(std::string objName, glm::vec2 pos, bool isLock)
			:ObjData(objName, pos, EntityType::SideDoor), isLock(isLock){}
		PM::GameObject* DataToObj() override;
		Json::Value DataToValue() override;
	protected:
		bool isLock{false};
	};

	class WallData : public ObjData
	{
	public:
		WallData(std::string objName, glm::vec2 pos, glm::vec2 size)
			:ObjData(objName, pos, EntityType::Wall), size_(size) {}
		PM::GameObject* DataToObj() override;
		Json::Value DataToValue() override;
	protected:
		glm::vec2 size_;
	};

	
	class KeyData : public ObjData
	{
	public:
		KeyData(std::string objName, glm::vec2 pos, std::string target)
			:ObjData(objName, pos, EntityType::Key), Target(target) {}
		PM::GameObject* DataToObj() override;
		Json::Value DataToValue() override;
	protected:
		std::string Target{ "Empty" };
		bool isDown{ false };
	};

	
	class ClothesData : public ObjData
	{
	public:
		ClothesData(std::string objName, glm::vec2 pos, DisguiseList dis)
			:ObjData(objName, pos, EntityType::Clothes), dis(dis) {}
		PM::GameObject* DataToObj() override;
		Json::Value DataToValue() override;
	protected:
		DisguiseList dis{DisguiseList::Guard};
	};

	class LazerData : public ObjData
	{
	public:
		LazerData(std::string objName, glm::vec2 pos, double onOffTime, glm::vec4 movable)
			:ObjData(objName, pos, EntityType::Lazer), onOffTime(onOffTime),xMovable(movable) {}
		void Draw(math::TransformMatrix& displayMatrix) override;
		PM::GameObject* DataToObj() override;
		Json::Value DataToValue() override;
	protected:
		double onOffTime = 0;
		glm::vec4 xMovable{};
	};
	class VentData : public ObjData
	{
	public:
		VentData(std::string objName, glm::vec2 pos, double xsize) : xSize(xsize),
			ObjData(objName, pos, EntityType::Vent_obj) {}// pos.x = xpos pos.y = floor
		virtual ~VentData() {}
		void Draw(math::TransformMatrix& displayMatrix) override;
		PM::GameObject* DataToObj() override;
		Json::Value DataToValue() override;
	protected:
		double xSize{0};
	};
	
	
	class NPCData : public ObjData
	{
	public:
		NPCData(std::string objName, glm::vec2 position, EntityType type, glm::vec2 point1, glm::vec2 point2) : point1(point1), point2(point2),
			ObjData(objName, position, type){}

		void Draw(math::TransformMatrix& displayMatrix) override;
		PM::GameObject* DataToObj() override;
		Json::Value DataToValue() override;
	protected:
		
		glm::vec2 point1;
		glm::vec2 point2;
	};
	

}

