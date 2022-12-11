/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "../Engine.h"
#include "../Collision.h"
#include "Mouse.h"
PM::UI::Mouse::Mouse() : UIEntity("Mouse")
{
	SetSize({20,20});
	AddGOComponent(new RectCollision({{-10,-10},{10,10}}, this));
}

void PM::UI::Mouse::Update(double dt)
{
	glm::vec2 pos = Engine::GetInput().GetDeviceMousePos();

	 SetPosition(pos);
	UpdateGOComponents(dt);
}

void PM::UI::Mouse::Draw(math::TransformMatrix& /*displayMatrix*/) const
{
	glm::vec2 pos = GetPosition();
	glm::vec2 msize = GetSize();
	PM::push_settings();
	PM::apply_NDC();
	PM::draw_rect(pos.x - msize.x/2, pos.y - msize.y/2, msize.x, msize.y);
	PM::pop_settings();
}
/*
void PM::UI::WorldMouse::CollisionStay(Entity* en)
{
	collidedObjPtr = en;
}

void PM::UI::WorldMouse::CollisionExit(Entity* en)
{
	if(collidedObjPtr == en)
	{
		collidedObjPtr = nullptr;
	}
}
PM::UI::WorldMouse::WorldMouse() : Mouse(){}


void PM::UI::WorldMouse::Update(double dt)
{
	SetPosition(Engine::GetInput().GetWorldMousePos());
	UpdateGOComponents(dt);
}

void PM::UI::WorldMouse::Draw(math::TransformMatrix& srt) const
{
	glm::vec2 pos = GetPosition();
	glm::vec2 size = GetSize();
	PM::push_settings();
	PM::apply_matrix(PM::getCameraMat());
	PM::set_fill_color(255, 0, 0);
	PM::draw_rect(pos.x - size.x / 2, pos.y - size.y / 2, size.x, size.y);
	PM::pop_settings();
}
*/