/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectManager.cpp
Project: CS230
Author: Kevin Wright
Creation date: 2/14/2021
-----------------------------------------------------------------*/
#include "Engine.h"        //LogEvent
#include "TransformMatrix.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Collision.h"

void PM::GameObjectManager::Add(GameObject* obj) {
	gameObjects.push_back(obj);
}

PM::GameObjectManager::~GameObjectManager() {
	for (GameObject* obj : gameObjects) {
		delete obj;
	}
	gameObjects.clear();
}

void PM::GameObjectManager::Update(double dt) {
	std::vector<GameObject*> destroyList;
	for (GameObject* object : gameObjects) {
		object->Update(dt);
		if (object->ShouldDestroy() == true) {
			destroyList.push_back(object);
		}
	}
	for (GameObject* object : destroyList) {
		gameObjects.remove(object);
		delete object;
	}
}

void PM::GameObjectManager::DrawAll(math::TransformMatrix& cameraMatrix) {
	for (GameObject* object : gameObjects) {
		object->Draw(cameraMatrix);
	}
}

void PM::GameObjectManager::CollideTest() {

	for (GameObject* objectA : gameObjects) {
		for (GameObject* objectB : gameObjects) {
			if (objectA != objectB && objectA->CanCollideWith(objectB->GetEntityType()) && objectB->GetGOComponent<PM::Collision>() != nullptr) {
					objectA->UpdateCollision(objectB);
					//std::cout << "a";
			}
			
			//if (objectA != objectB)
			//{
			//	std::cout << "a";
			//}
			//if (objectA->CanCollideWith(objectB->GetEntityType()))
			//{
			//	std::cout << "b";
			//}
			//if (objectB->GetGOComponent<PM::Collision>() != nullptr)
			//{
			//	std::cout << "c";
			//}
		}
	}
}
