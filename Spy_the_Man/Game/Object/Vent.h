/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <list>
#include "../../Engine/GameObject.h"
#include<glm/glm.hpp>
#include "../Engine/World.h"

namespace PM
{
	class Texture;
}

class VentPipe : public PM::GameObject
{
public:
	VentPipe(std::string name, double Xpos, double Xsize, double floor);
	VentPipe(std::string name, glm::vec2 pos, double xsize);
	~VentPipe();


	void Update(double dt) override;
	void Draw(math::TransformMatrix& displayMatrix) const override;

	EntityType GetEntityType() const override;
	std::string GetEntityTypeName() const override;
private:

};

class VentDoor : public PM::GameObject
{
public:
	VentDoor(std::string name, glm::vec2  pos, std::string targetName, glm::vec2 size = { 100,110 }, std::string filepath = "assets/ventDoor.png");
	~VentDoor();


	void Update(double dt) override;
	GameObject* GetTarget();

	void CollisionStay(Entity*) override;
	bool CanCollideWith(EntityType objectBType) override;

	EntityType GetEntityType() const override;
	std::string GetEntityTypeName() const override;
private:

	std::string TargetName;
	GameObject* TargetPtr{ nullptr };
};
