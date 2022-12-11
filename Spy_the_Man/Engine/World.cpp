/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
Creation date: 10/12/2021
-----------------------------------------------------------------*/
#include <algorithm>
#include "Engine.h"
#include "World.h"

#include <set>


#include "Collision.h"
#include "../Game/Object/Room.h"
#include "GameObjectManager.h"
#include "NPC_base.h"
#include "../Game/Basic_NPC.h"

#include "../Game/Object/ButtonManager.h"

/// <summary>
/// World class Helper function
/// </summary>

std::vector<int> PM::World::GetAroundChunkNumber(int num)
{
	std::vector<int> numbers{ num };
	int leftNum = num - 1;
	int rightNum = num + 1;

	glm::ivec2 WCS = GetChunkSize();
	if (rightNum < GetChunkSizeMax())
	{
		if (rightNum % WCS.x != 0)
		{
			numbers.push_back(rightNum);
		}
	}
	if (leftNum >= 0)
	{
		if (leftNum % WCS.x != WCS.x - 1)
		{
			numbers.push_back(leftNum);
		}
	}
	return numbers;
}

std::vector<int> PM::World::GetAroundChunkNumber(glm::vec2 pos)
{
	return GetAroundChunkNumber(GetChunkNumber(pos));
}

std::vector<int> PM::World::GetAroundChunkNumber(GameObject* obj)
{
	return GetAroundChunkNumber(obj->GetPosition());
}

inline bool haveDoorobj(PM::Chunk* chunk)
{
	std::list<PM::GameObject*> objs = chunk->GetObjects();
	std::vector<PM::GameObject*> result;

	for(PM::GameObject*& obj : objs)
	{
		if(obj->GetEntityType() == EntityType::SideDoor || obj->GetEntityType() == EntityType::Wall)
		{
			if (obj->GetGOComponent<PM::Collision>() != nullptr)
			{
				return true;
			}
		}
	}
	return false;
}

std::vector<int> PM::World::GetFrontChunkNumber(GameObject* obj)
{
	//math::rect2 objrect = obj->GetGOComponent<RectCollision>()->GetWorldCoorRect();
	
	int R_num = GetChunkNumber(obj->GetPosition() + glm::vec2{ obj->GetSize().x,0 });
	int L_num = GetChunkNumber(obj->GetPosition() );
	std::vector<int> numbers;
	if(L_num == R_num)
	{
		numbers.push_back(L_num);
	}
	else
	{
		numbers.push_back(L_num);
		numbers.push_back(R_num);
	}
	glm::ivec2 WCS = GetChunkSize();
	
	if(obj->IsFaceLeft())
	{
		for (int i = L_num - 1; i > L_num - 3; --i)
		{

			if (i >= 0 && i % WCS.x != WCS.x - 1)
			{
				if(haveDoorobj(&worldChunk[obj->GetLayer()][i]))
				{
					break;
				}
				else
				{
					numbers.push_back(i);
				}
			}
			else { break; }
		}	
	}
	else
	{
			for (int i = R_num + 1 ; i < R_num + 3; ++i)
			{

				if (i < worldChunkSizeMax && i % WCS.x !=0)
				{
					if (haveDoorobj(&worldChunk[obj->GetLayer()][i-1]))
					{
						break;
					}
					numbers.push_back(i);
				}
				else { break; }
			}
		
	}
	return numbers;
}


int PM::World::GetFloorNumber(int index)
{
	return index / worldChunkSize.x;
}

int PM::World::GetFloorNumber(glm::vec2 pos)
{
	int chunk_num = GetChunkNumber(pos);
	return chunk_num / worldChunkSize.x;
}

int PM::World::GetFloorNumber(Chunk* chunk)
{
	return GetFloorNumber(chunk->GetPosition());
}

int PM::World::GetFloorNumber(GameObject* obj)
{
	return GetFloorNumber(obj->GetPosition());
}

glm::vec2& PM::World::GetFloorSize(GameObject* obj) {
	return floorsize[GetFloorNumber(obj)];
}

std::vector<PM::Chunk*> PM::World::GetAllFloorChunk(int floorNumber, int layer)
{
	std::vector<PM::Chunk*> Ptrchunks;
	int floorStart = floorNumber * worldChunkSize.x;
	int floorEnd = (floorNumber + 1) * worldChunkSize.x;
	if(floorEnd > worldChunkSizeMax)
	{
		Engine::GetLogger().LogDebug("world GetAllFloorChunk number is over the chunkMaxSize");
		return Ptrchunks;
	}
	for(int i = floorStart; i < floorEnd; i++)
	{
		Ptrchunks.push_back(&worldChunk[layer][i]);
	}
	return Ptrchunks;
}


PM::Chunk& PM::World::GetChunk(GameObject* obj)
{
	int Cnum = GetChunkNumber(obj);

	return worldChunk[obj->GetLayer()][Cnum];
	
}

PM::Chunk& PM::World::GetChunk(int layer, glm::vec2 pos)
{
	int Cnum = GetChunkNumber(pos);

	return worldChunk[layer][Cnum];
}


std::vector<PM::Chunk*> PM::World::GetAllFloorChunk(GameObject* obj)
{
	return GetAllFloorChunk(GetFloorNumber(obj), obj->GetLayer());
}

std::vector<PM::Chunk*> PM::World::GetAllFloorChunk(glm::vec2 pos, int layer)
{
	return GetAllFloorChunk(GetFloorNumber(pos), layer);
}

std::vector<PM::Chunk*> PM::World::GetAllFloorChunk(Room* room)
{
	return GetAllFloorChunk(GetFloorNumber(room), room->GetLayer());
}

std::vector<PM::Chunk*> PM::World::GetAllFloorChunk(Chunk* chunk, int layer)
{
	return GetAllFloorChunk(GetFloorNumber(chunk), layer);
}


std::vector<PM::Chunk*> PM::World::GetChunk(Room* room)
{
	std::vector<Chunk*> c;

	std::vector<int> cNums = GetChunkNumber(room);
	int layer = room->GetLayer();
	for (int cNum : cNums)
	{
		c.push_back(&worldChunk[layer][cNum]);
	}

	return c;
}

PM::Room* PM::World::GetRoom(GameObject* obj)
{
	int cNum = GetChunkNumber(obj);

	return worldChunk[obj->GetLayer()][cNum].roomPtr;
}

PM::Room* PM::World::GetRoom(int layer, int chunk)
{
	return worldChunk[layer][chunk].roomPtr;
}

PM::GameObject* PM::World::GetObject(std::string n)
{
	for(auto* obj: gameObjects)
	{
		if(obj->GetName() == n)
		{
			return obj;
		}
	}
	return nullptr;
}


int PM::World::GetChunkNumber(glm::vec2 pos)
{
	glm::ivec2 chunk_pos = { pos.x / ROOMXSIZE, pos.y / (ROOMYSIZE) };
	glm::ivec2 chunk_size = GetChunkSize();

	int number = chunk_pos.x + chunk_size.x * chunk_pos.y;
	if (0 > number || number >= chunk_size.x * chunk_size.y)
	{
		Engine::GetLogger().LogError("0 > number || number >= CHUNKMAX");
		std::cout << number;
		return -1;
	}
	return number;
}

std::vector<int> PM::World::GetChunkNumber(Room* room)
{
	const auto r_size = room->GetSize();
	const auto r_pos = room->GetPosition();

	int chunkNumber = GetChunkNumber(r_pos);
	int chunkValue = static_cast<int>(r_size.x) / ROOMXSIZE; /*The number of chunks in the room.*/
	std::vector<int> numbers;
	for (int i = chunkNumber; i < chunkNumber + chunkValue; ++i)
	{
		numbers.push_back(i);
	}
	return numbers;
}

int PM::World::GetChunkNumber(GameObject* obj)
{
	return GetChunkNumber({ obj->GetPosition().x + obj->GetSize().x / 2,obj->GetPosition().y + obj->GetSize().y / 2 });
}



/// <summary>
/// Chunk definition
/// </summary>


PM::Chunk::Chunk(glm::ivec2 chunkNumber) {
	SetChunkNumber(chunkNumber);
}

void PM::Chunk::SetChunkNumber(glm::ivec2 chunkNumber_)
{
	this->chunkNumber = chunkNumber_;
	size = { ROOMXSIZE,ROOMYSIZE };
	position = { chunkNumber_.x * ROOMXSIZE, chunkNumber_.y * (ROOMYSIZE) };
}

void PM::Chunk::draw(math::TransformMatrix& cameraMatrix) const
{
	PM::push_settings();
	PM::apply_matrix(cameraMatrix.matrix);
	PM::rect_corner();
	PM::set_line_width(5);
	PM::set_line_color(0, 0, 0, 255);
	PM::no_fill();
	PM::draw_rect(position.x, position.y, size.x, size.y);
	PM::pop_settings();

}


/// <summary>
/// World definition
/// </summary>

PM::World::World(glm::ivec2 world_max_size) : Component("World"),
worldChunkSize(world_max_size), worldChunkSizeMax(world_max_size.x * world_max_size.y) ,
drawChunkKey(PM::InputKey::Keyboard::C),
layerKey(InputKey::Keyboard::L),
drawAllObjectKey(InputKey::Keyboard::P)
{
	for (int i = 0; i < LAYERMAX; ++i)
	{
		std::vector<Chunk> chunks;
		for (int j = 0; j < worldChunkSizeMax; ++j)
		{
			chunks.push_back(Chunk(glm::ivec2{ j % worldChunkSize.x,j / worldChunkSize.x }));
		}
	
		worldChunk.push_back(chunks);
	}
}

PM::World::~World()
{
	for (GameObject* obj : gameObjects) {
		if(obj == nullptr)
		{
			Engine::GetLogger().LogError("Error: obj is nullptr");
		}
		delete obj;
	}

	for(Room* ro : rooms)
	{
		delete ro;
	}
	gameObjects.clear();
}

void PM::World::Add(GameObject* obj, int layer)
{
	int chunkNumber = GetChunkNumber(obj->GetPosition()); // obj must need to their layer location
	if (chunkNumber < 0)
	{
		Engine::GetLogger().LogError("Faild World::GOM_To_Chunk() - posToChunk return nagative value");
		return;
	}
	obj->SetLayer(layer);
	gameObjects.push_back(obj);

	worldChunk[/*obj layer location*/obj->GetLayer()][chunkNumber].objs.push_back(obj);
	
	if (playerPtr == nullptr) //automatically set player
	{
		if (obj->GetEntityType() == EntityType::Player)
		{
			playerPtr = obj;
			return;
		}
	}
}

void PM::World::AddRoom(PM::Room* room, int layer_num)
{
	const auto r_size = room->GetSize();
	const auto r_pos = room->GetPosition();
	//check room size unit
	if ((int)r_size.x % ROOMXSIZE != 0)
	{
		Engine::GetLogger().LogError("Room Size have a problem");
		return;
	}
	if (r_pos.x < 0 || r_pos.y < 0)
	{
		Engine::GetLogger().LogError("Room Position can't be negative value");
		return;
	}
	if (layer_num < 0 || layer_num > LAYERMAX)
	{
		Engine::GetLogger().LogError("Layer can't be " + std::to_string(layer_num));
		return;
	}
	int chunkNumber = GetChunkNumber(r_pos);
	int chunkValue = (int)r_size.x / ROOMXSIZE; /*The number of chunks in the room.*/

	for (int i = chunkNumber; i < chunkNumber + chunkValue; ++i)
	{
		if (worldChunk[layer_num][i].roomPtr != nullptr)
		{
			Engine::GetLogger().LogError("Room" + worldChunk[layer_num][i].roomPtr->GetName() + "already setted. Failed Set Room: " + room->GetName());
			return;
		}
	}
	room->SetLayer(layer_num);
	rooms.push_back(room);
	for (int i = chunkNumber; i < chunkNumber + chunkValue; ++i) {
		worldChunk[layer_num][i].roomPtr = room;
		//std::list<GameObject*>& chunkobj = worldChunk[layer_num][i].GetObjects();
	}

	int floor = static_cast<int>(r_pos.y) / ROOMYSIZE;
	if (floorsize.find(floor) == floorsize.end()) {
		floorsize[floor] = { r_pos.x, r_pos.x + r_size.x };
	}
	else {
		if (floorsize[floor].x > r_pos.x) {
			floorsize[floor].x = r_pos.x;
		}
		if (floorsize[floor].y < r_pos.x + r_size.x) {
			floorsize[floor].y = r_pos.x + r_size.x;
		}
	}

	room = nullptr;
}


void PM::World::DrawAll(math::TransformMatrix& cameraMatrix) const
{
	for (auto& room : rooms)
	{
		room->DrawRoomImage(cameraMatrix);
		
	}
	if(drawAllObject)
	{
		for (auto& obj : gameObjects)
		{
				obj->Draw(cameraMatrix);
		}
	}
	else
	{
		for (auto& obj : gameObjects)
		{
			if (obj->GetEntityType() != EntityType::Worker_NPC && obj->GetEntityType() != EntityType::Enemy_NPC)
			{
				obj->Draw(cameraMatrix);
			}
			else
			{
				if(dynamic_cast<Basic_NPC*>(obj)->IsDetactSound() == true)
				{
					obj->Draw(cameraMatrix);
				}
			}
		}
		for (auto& room : rooms)
		{
			if (room->GetWasValid())
			{
				room->DrawEnemy(cameraMatrix);
			}
			else if (isDetectNPC == true)
			{
				room->DrawEnemy(cameraMatrix);
				room->DrawBlackRect(cameraMatrix);
			}
			else
			{
				room->DrawBlackRect(cameraMatrix);
			}
		}
	}
	DetectNPCEffect(cameraMatrix);

	if (currentLayer == playerPtr->GetLayer())
	{
		playerPtr->Draw(cameraMatrix);
	}

	if (drawChunk == true)
	{
		const std::vector<Chunk>& ch = worldChunk[currentLayer];
		for (const Chunk& chunk : ch)
		{
			chunk.draw(cameraMatrix);
		}
	}
}

void PM::World::DetectNPCEffect(math::TransformMatrix& cameraMatrix) const
{
	if (isDetectNPC == true) //DetectNPC Effect
	{
		push_settings();
		set_line_width(5);
		set_line_color(255, 255, 255, 100);
		no_fill();
		apply_matrix(cameraMatrix.matrix);
		glm::vec2 playerpos = playerPtr->GetCenter();
		draw_ellipse(playerpos.x, playerpos.y, detectEffectSize, detectEffectSize);
		pop_settings();
	}
}


void PM::World::Update(double dt)
{
	if (!stopUpdate)
	{
		std::vector<GameObject*> destroyList;
		std::vector<OldObjData> updateChunkList;

		for (GameObject* object : gameObjects) {
			glm::vec2 oldpos = object->GetPosition();
			int oldlayer = object->GetLayer();

			object->Update(dt);
			if (object->ShouldDestroy() == true) {
				destroyList.push_back(object);
			}
			if (object->ShouldUpdateChunk() == true)
			{
				updateChunkList.push_back({ object,oldpos,oldlayer });
			}
		}
		for (Room* room : rooms)
		{
			room->Update(dt);
		}
		currentLayer = playerPtr->GetLayer();
		for (OldObjData& obj : updateChunkList) //update anything that moved
		{
			int oldLayer = obj.object->GetLayer();
			int oldChunkNumber = GetChunkNumber(obj.oldpos);
			int newLayer = obj.object->GetLayer();
			int newChunkNumber = GetChunkNumber(obj.object->GetPosition());
			if (oldLayer != newLayer || newChunkNumber != oldChunkNumber)
			{
				worldChunk[oldLayer][oldChunkNumber].GetObjects().remove(obj.object);
				worldChunk[newLayer][newChunkNumber].GetObjects().push_back(obj.object);
			}
			obj.object->UpdateChunkFinished();
		}
		for (GameObject* object : destroyList) {
			int Layer = object->GetLayer();
			int ChunkNumber = GetChunkNumber(object->GetPosition());
			auto bm = object->GetGOComponent<ButtonManager>();
			if(bm != nullptr)
			{
				bm->Activate(false);
			}
			worldChunk[Layer][ChunkNumber].objs.remove(object); //chunk obj delete
			gameObjects.remove(object); // gameObj delete

			delete object;
			object = nullptr;
		}
#ifdef _DEBUG
		if (drawChunkKey.IsKeyReleased() == true)
		{
			drawChunk = !drawChunk;
		}
		if(drawAllObjectKey.IsKeyReleased() == true)
		{
			drawAllObject = !drawAllObject;
		}
#endif
		if (isDetectNPC == true)
		{
			detectEffectSize += static_cast<float>(8000. * dt);
		}
		else
		{
			detectEffectSize = 0;
		}
	}
}




void PM::World::CollideTest()
{
	for (GameObject*& objectA : gameObjects) 
	{

		for (GameObject*& objectB : gameObjects)
		{
			
				if (objectA != objectB && objectA->CanCollideWith(objectB->GetEntityType()) && objectB->GetGOComponent<PM::Collision>() != nullptr) {
					objectA->UpdateCollision(objectB);
				}
		}
		/*
		int objLayer = objectA->GetLayer();
		int chunkNumber = GetChunkNumber(objectA->GetPosition());
		std::vector<int> sideChunk = GetAroundChunkNumber(chunkNumber);
		if(chunkNumber + worldChunkSize.x < worldChunkSizeMax)
		{
			std::vector<int> sideUpChunk;
			sideUpChunk = GetAroundChunkNumber(chunkNumber + worldChunkSize.x);
			sideChunk.insert(sideChunk.end(), sideUpChunk.begin(), sideUpChunk.end());
		}
		if(chunkNumber - worldChunkSize.x > 0)
		{
			std::vector<int> sideDownChunk;
			sideDownChunk = GetAroundChunkNumber(chunkNumber - worldChunkSize.x);
			sideChunk.insert(sideChunk.end(), sideDownChunk.begin(), sideDownChunk.end());
		}
		
		
		std::vector<Chunk>& wc = worldChunk[objLayer];
		for (int chunk : sideChunk)
		{
			std::list<GameObject*>& g_objs = wc[chunk].objs;
			for (GameObject* objectB : g_objs)
			{
				if (objectA != objectB && objectA->CanCollideWith(objectB->GetEntityType()) && objectB->GetGOComponent<PM::Collision>() != nullptr) {
					objectA->UpdateCollision(objectB);
				}
			}
		}
		*/
	}
}



std::vector<PM::GameObject*> PM::World::GetNearEnemy(unsigned int Left, unsigned int Right, unsigned int Up, unsigned int Down)
{
	int playerChunk = GetChunkNumber(playerPtr);
	int playerFloor = GetFloorNumber(playerPtr);
	int playercolumn = playerChunk % worldChunkSize.x;
	std::vector<PM::GameObject*> gos;
	int row = playerFloor - Down;
	int col = playercolumn - Left;
	if (row < 0) {
		row = 0;
	}
	if (col < 0) {
		col = 0;
	}
	for (row; row < playerFloor + static_cast<int>(Up) + 1; row++)
	{
		if (row >= worldChunkSize.y) { break; }
		for (col; col < playercolumn + static_cast<int>(Right) + 1; col++)
		{
			if (col >= worldChunkSize.x) { break; }
			for(auto go : worldChunk[playerPtr->GetLayer()][row * worldChunkSize.x + col].objs)
			{
				if(go->GetEntityType() == EntityType::Enemy_NPC && go->GetLayer() == playerPtr->GetLayer())
				{
					gos.push_back(go);
				}
			}
		}
	}
	return gos;
}

std::vector<PM::GameObject*> PM::World::GetNearNPC(unsigned Left, unsigned Right, unsigned Up, unsigned Down,PM::GameObject* obj)
{
	if(obj == nullptr)
	{
		obj = playerPtr;
	}
	int playerChunk = GetChunkNumber(obj);
	int playerFloor = GetFloorNumber(obj);
	int playercolumn = playerChunk % worldChunkSize.x;
	std::vector<PM::GameObject*> gos;
	int row = playerFloor - Down;
	int col = playercolumn - Left;
	if (row < 0) {
		row = 0;
	}
	if (col < 0) {
		col = 0;
	}
	for (row; row < playerFloor + static_cast<int>(Up) + 1; row++)
	{
		if (row >= worldChunkSize.y) { break; }
		for (col; col < playercolumn + static_cast<int>(Right) + 1; col++)
		{
			if (col >= worldChunkSize.x) { break; }
			for (auto go : worldChunk[playerPtr->GetLayer()][row * worldChunkSize.x + col].objs)
			{
				if ((go->GetEntityType() == EntityType::Worker_NPC ||go->GetEntityType() == EntityType::Enemy_NPC) && go->GetLayer() == obj->GetLayer())
				{
					gos.push_back(go);
				}
			}
		}
	}
	return gos;
}


std::vector<PM::GameObject*> PM::World::GetAllNPC()
{
	std::vector<PM::GameObject*> gos;
	for(auto go : gameObjects)
	{
		if (dynamic_cast<NPC_base*>(go) != nullptr && go->GetLayer() == playerPtr->GetLayer())
		{
			gos.push_back(go);
		}
	}
	return gos;
}

void PM::World::SetRoomVision(GameObject * obj)
{
	if (obj->GetLayer() == currentLayer)
	{
		int Cnum = GetChunkNumber(obj);
		std::set<Room*> rooms_;
		Room* r = GetChunk(currentLayer, Cnum).roomPtr;
		rooms_.insert(r);
		if (obj->IsFaceLeft())
		{
			for (int i = 0; i < 2; i++)
			{
				r = r->GetLeftRoom();
				if (r == nullptr) { break; }
				GameObject* rightobj = r->GetRightObj();
				if (rightobj != nullptr && rightobj->GetGOComponent<Collision>() != nullptr) { break; }
				rooms_.insert(r);
			}
		}
		else
		{
			for (int i = 0; i < 2; i++)
			{
				r = r->GetRightRoom();
				if (r == nullptr) { break; }
				GameObject* rightobj = r->GetLeftObj();
				if (rightobj != nullptr && rightobj->GetGOComponent<Collision>() != nullptr) { break; }
				rooms_.insert(r);
			}
		}
		for (Room* room : rooms_)
		{
			if (room != nullptr && room->GetValid() == false)
			{
				room->SetValid(true);
			}
		}
	}
}