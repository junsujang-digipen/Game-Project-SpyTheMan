/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Room.cpp
Project: Spy the Man
Author: Seongwon Jang
Creation date: 10/12/2021
-----------------------------------------------------------------*/
#include "Room.h"



#include "ButtonManager.h"
#include "../../Engine/Poodle.h"
#include "../../Engine/Engine.h"
#include "../../Engine/World.h"
#include "../../Game/Player.h"
PM::Room::Room(std::string name, glm::vec2 pos, glm::vec2 size, GameObject* left, GameObject* right, std::string image, int security) : GameObject(name, pos),Left(left), Right(right), securityLevel(security)
{

	ChangeState(&state_nothing);
	SetSize(size);
	AddGOComponent(new PM::RectCollision({ {0,0},size }, this));
	roomImage = Engine::GetTextureManager().Load(image);
}

PM::Room::~Room()
{
	// Room will not delete game Object. Room only Check that what object is in
}

void PM::Room::SetValid(bool valid)
{
	Valid = valid;
}

void PM::Room::Update(double dt) {
	GameObject::Update(dt);
	wasValid = Valid;
	Valid = false;

	
}

PM::Room* PM::Room::GetLeftRoom()
{
	World* world = Engine::GetGSComponent<PM::World>();
	int roomcn = world->GetChunkNumber(this->GetPosition());
	int newcn = roomcn - 1;
	int ySize = static_cast<int>(world->GetChunkSize().y);
	if (newcn < 0 || newcn / ySize != roomcn / ySize)
	{
		return nullptr;
	}
	return world->GetWorldChunk(GetLayer())[newcn].roomPtr;
}

PM::Room* PM::Room::GetRightRoom()
{
	World* world = Engine::GetGSComponent<PM::World>();
	int cn = world->GetChunkNumber(this->GetPosition() + glm::vec2{this->GetSize().x - 1,0});
	if(cn >= world->GetChunkSizeMax() - 1  || cn % static_cast<int>(world->GetChunkSize().x) == world->GetChunkSize().x - 1)
	{
		return nullptr;
	}
	return world->GetWorldChunk(GetLayer())[cn+1].roomPtr;
}

void PM::Room::DrawRoomImage(math::TransformMatrix& displayMatrix)
{
	glm::vec2 pos = GetPosition();
	glm::vec2 Rsize = GetSize();
	//roomImage->Draw(displayMatrix, pos, size);
	PM::push_settings();
	no_outline();
	PM::apply_matrix(displayMatrix.matrix);
	PM::set_fill_color(255, 255, 255);
	PM::draw_rect(pos.x, pos.y, Rsize.x, Rsize.y);
	PM::set_fill_color(100, 100, 100);
	PM::draw_rect(pos.x,pos.y, Rsize.x,WALLTHICKNESS); //under
	PM::draw_rect(pos.x, pos.y - WALLTHICKNESS + ROOMYSIZE, Rsize.x, WALLTHICKNESS); // top
	PM::draw_rect(pos.x, pos.y + WALLTHICKNESS + 300, WALLTHICKNESS, ROOMYSIZE - 300 - WALLTHICKNESS); // left
	PM::draw_rect(pos.x + Rsize.x - WALLTHICKNESS, pos.y + WALLTHICKNESS + 300, WALLTHICKNESS, ROOMYSIZE - 300 - WALLTHICKNESS); // right
	
	PM::pop_settings();

}


void PM::Room::DrawEnemy(math::TransformMatrix& displayMatrix)
{
	
	for(auto& e : Engine::GetGSComponent<PM::World>()->GetNearNPC(100, 100,0,0,this))
	{
		if (DoesCollideWith(e)) {
			e->Draw(displayMatrix);
		}
	}
}
void PM::Room::DrawBlackRect(math::TransformMatrix& displayMatrix)
{
	glm::vec2 pos = GetPosition();
	glm::vec2 size_ = GetSize();
	PM::push_settings();
	PM::set_fill_color(0, 0, 0, 80);
	PM::apply_matrix(displayMatrix.matrix);
	PM::no_outline();
	PM::draw_rect(pos.x, pos.y, size_.x, size_.y); //under

	PM::pop_settings();
}


const std::vector<PM::GameObject*> PM::Room::Objects()
{
	std::vector<Chunk*> chunks = Engine::GetGSComponent<World>()->GetChunk(this);
	int chunksize = static_cast<int>(chunks.size());
	std::vector<PM::GameObject*> objs;

	for(int i = 0; i < chunksize; ++i)
	{
		std::list<GameObject*> cObj = chunks[i]->GetObjects();
		objs.insert(objs.end(), cObj.begin(), cObj.end());
	}
	return objs;
}


