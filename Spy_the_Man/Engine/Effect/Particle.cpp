/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "Particle.h"
#include "../Poodle.h"
#include "../Engine/Engine.h"
Particle::Particle(glm::vec2 Position, double Angle, double Time, double Speed, glm::vec2 size)
	: position(Position), angle(Angle), time(Time), speed(Speed), size(size)
{}

void Particle::SetPosition(glm::vec2 pos)
{
	position = pos;
}

void Particle::SetVelocity(glm::vec2 vel)
{
	velocity = vel;
}

void Particle::SetAngle(double a)
{
	this->angle = a;
}

void Particle::SetSpeed(double s)
{
	this->speed = s;
}




void Particle::Update(double dt)
{
	timer += dt;
	position += velocity * dt;
	if (timer >= time)
	{
		isDead = true;
	}
}

void Particle::Draw(math::TransformMatrix& displayMatrix)
{
	PM::push_settings();
	PM::rect_corner();
	PM::set_fill_color(255, 255, 255, 255);
	PM::apply_rotate(glm::degrees(angle), position.x + size.x /2.f, position.y + size.y/2.f);
	PM::apply_matrix(displayMatrix.matrix);
	PM::set_fill_color(color.x, color.y, color.z, color.a);
	PM::draw_rect(position.x, position.y, size.x , size.y);
	PM::pop_settings();
}



electricParticle::electricParticle(glm::vec2 Position, double Angle, double Time, double Speed, glm::vec2 size)
	: Particle(Position, Angle, Time, Speed, size)
{
	color = yellow;
	velocity = { cos(angle) * speed,sin(angle) * speed };
}


void electricParticle::Update(double dt)
{
	velocity.y -= static_cast<float>(gravity) * 2.f * dt;
	angle = atan2(velocity.y, velocity.x);
	Particle::Update(dt);
}

InvisibleParticle::InvisibleParticle(glm::vec2 Position, double Angle, double Time, double Speed, glm::vec2 size)
	: Particle(Position, Angle, Time, Speed, size)
{
	color = {255,255,255,155};
	velocity = { cos(angle) * speed,sin(angle) * speed };
}

void InvisibleParticle::Update(double dt)
{
	Particle::Update(dt);
	color.a -= static_cast<float>(dt * 255);
	size += glm::vec2{  dt*80, dt*80 };
}
void InvisibleParticle::Draw(math::TransformMatrix& displayMatrix)
{
	PM::push_settings();
	PM::rect_corner();
	PM::no_outline();
	//PM::apply_rotate(glm::degrees(angle), position.x + size.x / 2.f, position.y + size.y / 2.f);
	PM::apply_matrix(displayMatrix.matrix);
	PM::set_fill_color(color.x, color.y, color.z, color.a);
	PM::draw_ellipse(position.x, position.y, size.x, size.y);
	PM::pop_settings();
}

ClearParticle::ClearParticle(glm::vec2 Position, double Angle, double Time, double Speed, glm::vec2 size)
	: Particle(Position,Angle,Time,Speed,size)
{

		color = { 255,Engine::GetRandomNum(0,255),50,255 };
	velocity = { cos(angle) * speed,sin(angle) * speed };
	//position = { Engine::GetRandomNum(0,1600),Engine::GetRandomNum(0,900) };
}

void ClearParticle::Update(double dt)
{
	Particle::Update(dt);
}

void ClearParticle::Draw(math::TransformMatrix& displayMatrix)
{
	PM::push_settings();
	PM::rect_corner();
	PM::no_outline();
	//PM::apply_rotate(glm::degrees(angle), position.x + size.x / 2.f, position.y + size.y / 2.f);
	PM::apply_matrix(displayMatrix.matrix);
	PM::set_fill_color(color.x, color.y, color.z, color.a);
	PM::draw_ellipse(position.x, position.y, size.x, size.y);
	PM::pop_settings();
}
