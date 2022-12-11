/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "../Engine.h"
#include "UIEntity.h"
#include "Mouse.h"

PM::UI::UIEntity::UIEntity(std::string name, UILayer layer)
	:Entity(name,{0,0}), layer(layer)
{
	if (GetName() == "Mouse") return;

		Engine::GetGSComponent<UIManager>()->AddUI(this, layer);

}


PM::UI::UIEntity::UIEntity(std::string name, glm::vec2 pos, glm::vec2 size, UILayer layer, bool valid) : Entity(name,pos, size), valid(valid),
layer(layer)
{
	Engine::GetGSComponent<UIManager>()->AddUI(this, layer);

}

void PM::UI::UIEntity::Setvalid(bool v)
{
	if (valid != v)
	{
		valid = v;
	}
}


void PM::UI::UIEntity::UpdateCollision(Mouse* mouse)
{
	if(DoesCollideWith(mouse) == true && mouse->GetCollidedObj() == nullptr)
	{
		mouse->SetCollidedObj(this);
	}
	else if(isCollided == true)
	{
		CollisionExit(mouse);
		isCollided = false;
	}
}

