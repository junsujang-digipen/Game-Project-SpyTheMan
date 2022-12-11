/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include <vector>
#include "Effect.h"
#include "../Engine.h"



template <typename T> //T must be particle
void Effect::Add()
{
	for (int i = 0; i < amount; i++)
	{
		if (MaxParticle <= particles.size()) { break; }
	glm::vec2 pos;
	if (startArea.bottomLeft().x == startArea.topRight().x) { pos.x = startArea.bottomLeft().x; }
	else { pos.x = (float)Engine::GetRandomNum((double)startArea.bottomLeft().x, (double)startArea.topRight().x); }
	if (startArea.bottomLeft().y == startArea.topRight().y) { pos.y = startArea.bottomLeft().y; }
	else { pos.y = (float)Engine::GetRandomNum((double)startArea.bottomLeft().y, (double)startArea.topRight().y); }
		
	double angle;
	if (minAngle == maxAngle) { angle = minAngle; }
	else { angle = Engine::GetRandomNum((double)minAngle, (double)maxAngle); }
		
	double speed;
	if (minSpeed == maxSpeed) { speed = minSpeed; }
	else { speed = Engine::GetRandomNum((double)minSpeed, (double)maxSpeed); }
		
	glm::vec2 size;
	if (minSize.x == maxSize.x) { size.x = minSize.x; }
	else { size.x = (float)Engine::GetRandomNum((double)minSize.x, (double)maxSize.x); }

	if (minSize.y == maxSize.y) { size.y = minSize.y; }
	else { size.y = (float)Engine::GetRandomNum((double)minSize.y, (double)maxSize.y); }

	double particleTime;
	if (minParticleTime == maxParticleTime) { particleTime = minParticleTime; }
	else { particleTime = Engine::GetRandomNum((double)minParticleTime, (double)maxParticleTime); }
		
	particles.push_back(new T(pos, angle, particleTime, speed, size));
	}
}

void Effect::Draw(math::TransformMatrix& displayMatrix)
{
	for (auto p : particles)
	{
		p->Draw(displayMatrix);
	}
}

void Effect::Update(double dt)
{
	std::vector<Particle*> destroyList;
	for(Particle* particle : particles)
	{
		particle->Update(dt);
		if(particle->IsDead())
		{
			destroyList.push_back(particle);
		}
	}
	if (effectTime <= effectTimer)
	{
		loop = false;
	}

	if( loop == true && loopTime <= loopTimer && particles.size() < MaxParticle)
	{
		MakeParticle();
		loopTimer = 0;
	}
	loopTimer += dt;
	effectTimer += dt;
	for(auto p : destroyList)
	{
		particles.remove(p);
		delete p;
	}
}

void Effect::Clear()
{
	for (auto particle : particles)
	{
		delete particle;
	}
	particles.clear();
}

electricEffect::electricEffect(math::rect2 startArea)
{
	this->startArea = startArea;
	MaxParticle = 150;
	amount = 18;
	loop = true;
	loopTime = 2;
	cycle = 3;
	effectTime = 10;
	SetParticleTime(1, 4);
	SetSpeed(400, 600);
	SetSize({8,4});
	SetAngle(0, 3.14);
	SetSpeed(100, 300);
	SetSize({6,10});
	MakeParticle();
}


void electricEffect::MakeParticle()
{
	Add<electricParticle>();
}

InvisibleEffect::InvisibleEffect(math::rect2 startArea)
{
	this->startArea = startArea;
	MaxParticle = 350;
	amount = 40;
	loop = false;
	loopTime = 2.;
	effectTime = 4.;
	SetParticleTime(2.);
	SetSpeed(30, 50);
	SetSize({ 20,20 }, {30,30});
	SetAngle(0, 6.28123 );
	SetSpeed(100, 300);
	MakeParticle();
}

void InvisibleEffect::MakeParticle()
{
	Add<InvisibleParticle>();
}


InvisibleEffect::~InvisibleEffect()
{
	
}

ClearEffect::ClearEffect(math::rect2 startArea)
{
	this->startArea = startArea;
	MaxParticle = 450;
	amount = 50;
	loop = true;
	loopTime = 3.;
	effectTime = 10.;
	SetParticleTime(10.);
	SetSize({ 10,10 }, { 30,30 });
	SetAngle(0.8,2.4);
	SetSpeed(500, 1000);
	MakeParticle();
}

void ClearEffect::MakeParticle()
{
	Add<ClearParticle>();
}

ClearEffect::~ClearEffect()
{
	
}
