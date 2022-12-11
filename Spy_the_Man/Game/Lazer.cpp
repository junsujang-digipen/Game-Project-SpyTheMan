/*--------------------------------------------------------------
File Name: Lazer.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 12/7/2021
-----------------------------------------------------------------*/
#include "Lazer.h"
#include "../Engine/Poodle.h"
#include "../Engine/Collision.h"
#include "../Engine/Declaration.h"

Lazer::Lazer(std::string name, glm::vec2 pos, glm::vec2 siz, double onOffTime, glm::vec4 xMovable):
	GameObject(name,pos,siz),LazerSize(static_cast<const int>(siz.x/5)), onOffTime(onOffTime),xMovable(xMovable)
{
	ChangeState(&state_nothing);
	//collision
	AddGOComponent(new PM::RectCollision(math::irect2{ {0, 0}, {LazerSize, ROOMYSIZE} },this));
}

Lazer::~Lazer()
{
}

void Lazer::Update(double dt)
{
	
	GameObject::Update(dt);
	//move itself?
	if (isPowerOff == false)
	{
		nowT += dt;
		if (onOffTime != 0. && onOffTime < nowT) {
			nowT = 0.;
			SetOnOffInverse();
			if (GetGOComponent<PM::RectCollision>() == nullptr) {
				this->AddGOComponent(new PM::RectCollision({ {0, 0}, {LazerSize, ROOMYSIZE } }, this));
			}
			else {
				RemoveGOComponent<PM::RectCollision>();
			}
		}

		if (xMovable.w != 0) {
			const glm::vec2 pos = GetPosition();
			if (pos.x > xMovable.y) {
				xMovable.w = -1.f;
			}
			else if (pos.x < xMovable.x) {
				xMovable.w = 1.f;
			}
			SetVelocity(glm::vec2{ xMovable.w * xMovable.z ,0 });
		}

	}
	else {
		isOn = false;
		SetVelocity({});
		if (GetGOComponent<PM::RectCollision>() != nullptr) {
			RemoveGOComponent<PM::RectCollision>();
		}
	}
}

void Lazer::Draw(math::TransformMatrix& displayMatrix) const
{
	GameObject::Draw(displayMatrix);
	PM::push_settings();
	PM::apply_matrix(displayMatrix.matrix);
	if (isOn == true) {
		PM::set_fill_color(255, 50, 50, 155);
		PM::no_outline();
		PM::draw_rect(GetPosition().x - 10, GetPosition().y, LazerSize + 20, ROOMYSIZE);
	}
	glm::vec2 Lsize = GetSize();
	PM::set_fill_color(0,0,0,255);
	PM::draw_rect(GetPosition().x - Lsize.x / 2 + LazerSize/2, GetPosition().y + ROOMYSIZE - Lsize.y, Lsize.x, Lsize.y);
	PM::draw_rect(GetPosition().x- Lsize.x / 2 + LazerSize / 2,GetPosition().y, Lsize.x, Lsize.y);
	PM::pop_settings();
}

EntityType Lazer::GetEntityType() const
{
	return EntityType::Lazer;
}

std::string Lazer::GetEntityTypeName() const
{
	return "Lazer";
}
