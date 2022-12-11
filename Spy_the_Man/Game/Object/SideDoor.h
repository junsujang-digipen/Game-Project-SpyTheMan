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
#include "../../Engine/Collision.h"
#include "../../Engine/Input.h"
#include "../../Engine/Font.h"
#include "../../Engine/Declaration.h"
class SideDoor : public PM::GameObject
	{
	public:
		SideDoor(std::string name, glm::vec2 pos, bool locked = false);
		~SideDoor();

		void Update(double dt) override;
		void Draw(math::TransformMatrix& displayMatrix) const override;
		void OpenAndClose();
		
		void UnLock();
		
		bool GetLockState() { return lock; }
		bool GetIsOpen() { return isOpen; }
		EntityType GetEntityType() const override { return EntityType::SideDoor; }
		std::string GetEntityTypeName() const override { return std::string("SideDoor"); }
		//PM::ObjData* MakeDataFile() const override;

		
	private:
		
		void SetCollision(bool);
		 std::string name;
		 glm::dvec2 drawRect;
		 math::irect2 Door_objPositionSize = { {0, 0}, {50, 300} };
		 PM::InputKey DoorCheckButton;
		 bool lock{ false };
		 bool isOpen{ false };
		 int lockLevel{1};	
	
		
	};
