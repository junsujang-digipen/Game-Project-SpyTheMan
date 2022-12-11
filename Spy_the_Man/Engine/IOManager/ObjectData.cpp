/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "../../Game/Player.h"
#include "../../Game/Object/Hide_Obj.h"
#include "../../Game/Object/SideDoor.h"
#include "../../Game/Object/Goal.h"
#include "../../Game/Object/Vent.h"
#include "ObjectData.h"


#include "../Engine.h"
#include "../../Game/Object/Room.h"
#include "../../Game/Object/Wall.h"
#include "../../Engine/Declaration.h"
#include "../../Game/Basic_NPC.h"
#include "../../Game/Lazer.h"
#include "../../Game/Task_AI_pointToPoint.h"
#include "../../Game/Object/Clothes.h"
#include "../../Game/Object/Key.h"
#include "../../Game/Object/Stair.h"
#include "../../Game/Object/Vent.h"
#include "../World.h"

PM::ObjData::ObjData(std::string objName) : objName(objName){}

PM::ObjData::ObjData(std::string objName, glm::vec2 position, EntityType type) : objName(objName), position(position), GOT(type) {}

Json::Value PM::ObjData::DataToValue()
{
	Json::Value objValue;
	objValue["TYPE"] = static_cast<int>(GOT);
	objValue["NAME"] = objName;
	objValue["POSITION"].append(position.x);
	objValue["POSITION"].append(position.y);
	return objValue;
}

PM::GameObject* PM::ObjData::DataToObj()
{
	switch (GOT) {
	case EntityType::Empty: break;
	case EntityType::Player: 
		return new Player(position);
		break;

	case EntityType::Hide_Obj: 
		return new Hide_Obj(GetName(), position); 
		break;

	
	//case EntityType::Wall: 
	//	return new Wall(GetName(), position);
	//	break;

	//case EntityType::Worker_NPC: break; //AI 정보

	//case EntityType::Enemy_NPC: break; // AI 정보

	case EntityType::Goal:
		return new Goal(position);
		break;
	//case EntityType::Switch: break; // 다양한 스위치 만들 예정이니 보류
	default: 
		return nullptr;
	}
	return nullptr;
}

void PM::ObjData::Draw(math::TransformMatrix& displayMatrix)
{
	std::string image_{ "EMPTY" };
	glm::vec2 size_{ 100,100 };
	glm::vec4 color{0,0,0,155};
	switch (GOT) {
	case EntityType::Player:
		color = { 0,255,0,155 };
		size_ = { 100,200 };
		break;
		case EntityType::SideDoor:
			size_ = SIDEDOORSIZE;
		break;
	case EntityType::Hide_Obj:
		image_ = HIDEOBJPATH;
		size_ = HIDEOBJSIZE;
		break;
		case EntityType::Stair:
			image_ = STAIRPATH;
			size_ = STAIRSIZE;
			break;

	case EntityType::Wall:
		color = { 100,100,100,155 };
		size_ = WALLSIZE;
		break;

		case EntityType::Key:
			image_ = KEYPATH;
			size_ = KEYSIZE;
			break;
		case EntityType::Clothes:
			image_ = CLOTHESPATH;
			size_ = CLOTHESSIZE;
			break;

		case EntityType::Lazer: 
			size_ = LAZERSIZE;
			break;
	case EntityType::Goal:
		image_ = GOALPATH;
		size_ = GOALSIZE;		
		break;

	default: break;
	}
	{
		PM::push_settings(); //what?

		if (image_ != "EMPTY")
		{
			PM::Image(image_);
			draw_image(image_, displayMatrix, position, size_);
		}
		else
		{
			apply_matrix(displayMatrix.matrix);
			set_fill_color(color.r, color.g, color.b, color.a);
			set_line_width(3);
			draw_rect(position.x, position.y, size_.x, size_.y);
		}
		pop_settings();
	}
}

PM::GameObject* PM::StairData::DataToObj()
{
	return new Stair(objName, position, Target,isDown);
}
Json::Value PM::StairData::DataToValue()
{
	Json::Value data = ObjData::DataToValue();
	data["TARGET"] = Target;
	data["ISDOWN"] = isDown;
	return data;
}

PM::GameObject* PM::SideDoorData::DataToObj()
{
	return new SideDoor(objName, position, isLock);
}
Json::Value PM::SideDoorData::DataToValue()
{
	Json::Value data = ObjData::DataToValue();
	data["LOCK"] = isLock;
	return data;
}

PM::GameObject* PM::WallData::DataToObj()
{
	return new Wall(objName, position, size_);
}

Json::Value PM::WallData::DataToValue()
{
	Json::Value data = ObjData::DataToValue();
	data["SIZE"].append(size_.x);
	data["SIZE"].append(size_.y);
	return data;
}


PM::GameObject* PM::KeyData::DataToObj()
{
	return new Key(objName,position,Target);
}
Json::Value PM::KeyData::DataToValue()
{
	Json::Value objValue;
	objValue["TYPE"] = static_cast<int>(GOT);
	objValue["NAME"] = objName;
	objValue["POSITION"].append(position.x);
	objValue["POSITION"].append(position.y);
	objValue["TARGET"] = Target;
	return objValue;
}


PM::GameObject* PM::ClothesData::DataToObj()
{
	return new Clothes(objName,position,dis);
}

Json::Value PM::ClothesData::DataToValue()
{
	Json::Value data = ObjData::DataToValue();
	data["DISGUISE"] = static_cast<int>(dis);
	return data;
}

void PM::LazerData::Draw(math::TransformMatrix& displayMatrix)
{
	PM::push_settings();
	PM::apply_matrix(displayMatrix.matrix);

	glm::vec2 Lsize = { 100,50 };
	PM::set_fill_color(0, 0, 0, 255);
	PM::draw_rect(GetPosition().x - Lsize.x / 2 + 20 / 2, GetPosition().y + ROOMYSIZE - Lsize.y, Lsize.x, Lsize.y);
	PM::draw_rect(GetPosition().x - Lsize.x / 2 + 20 / 2, GetPosition().y, Lsize.x, Lsize.y);
	PM::pop_settings();
}


PM::GameObject* PM::LazerData::DataToObj()
{
	return new Lazer(objName,position,LAZERSIZE,onOffTime,xMovable);
}

Json::Value PM::LazerData::DataToValue()
{
	Json::Value data = ObjData::DataToValue();
	data["TIME"] = onOffTime;
	data["MOVABLE"].append(xMovable.x);
	data["MOVABLE"].append(xMovable.y);
	data["MOVABLE"].append(xMovable.z);
	data["MOVABLE"].append(xMovable.w);
	return data;
}

void PM::VentData::Draw(math::TransformMatrix& displayMatrix)
{
	push_settings();
	glm::vec2 pos = {GetPosition().x, ROOMYSIZE - 125 - WALLTHICKNESS + ROOMYSIZE * position.y };
	glm::vec4 color = { 100,100,0,255 };
	apply_matrix(displayMatrix.matrix);
	set_fill_color(color.r, color.g, color.b, color.a);
	set_line_width(3);
	draw_rect(pos.x, pos.y, xSize, 125);
	pop_settings();
}

void PM::NPCData::Draw(math::TransformMatrix& displayMatrix)
{
	push_settings();
	glm::vec4 color;
	if(GOT == EntityType::Enemy_NPC)
	{
		color = { 255,0,0,255 };
	}
	else
	{
		color = { 0,0,255,255 };
	}
	
	apply_matrix(displayMatrix.matrix);
	set_line_color(color.r, color.g, color.b, color.a);
	set_line_width(5);
	draw_line(point1.x, point1.y, point2.x, point2.y);
	pop_settings();
}


PM::GameObject* PM::VentData::DataToObj()
{
	return new VentPipe(objName, position, xSize);
}

Json::Value PM::VentData::DataToValue()
{
	Json::Value data = ObjData::DataToValue();
	data["XSIZE"] = xSize;
	return data;
}

PM::GameObject* PM::NPCData::DataToObj()
{
	NPC_generator npcGenerator(Engine::GetGSComponent<PM::World>()->GetPlayerPtr());
	npcGenerator.setting(GOT, NPCLevelList::Normal, false);
	Basic_NPC* tempEnemy{ npcGenerator.generate(objName, position) };
	tempEnemy->AddTaskAI(new Task_AI_pointToPoint{ tempEnemy,point1,point2,true });
	return tempEnemy;
}

Json::Value PM::NPCData::DataToValue()
{
	Json::Value data = ObjData::DataToValue();
	data["POINT1"].append(point1.x);
	data["POINT1"].append(point1.y);
	data["POINT2"].append(point2.x);
	data["POINT2"].append(point2.y);
	return data;
}

