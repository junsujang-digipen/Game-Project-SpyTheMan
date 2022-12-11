/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <list>
#include "../Rect.h"
#include "Particle.h"

namespace math
{
	struct rect2;
}

class Effect
{
	friend class EffectManager;
public:

protected:
	std::list<Particle*> particles;
	
	math::rect2 startArea;
	double minAngle{ 0 };
	double maxAngle{ 0 };

	glm::vec2 minSize{ 0 };
	glm::vec2 maxSize{ 0 };
	double minSpeed{ 0 };
	double maxSpeed{ 0 };
	double minParticleTime{ 0 };
	double maxParticleTime{ 0 };
	double effectTime{ 1 };
	double effectTimer{ 0 };
	bool loop = false;
	double loopTime = 0;
	double loopTimer = 0;
	double MaxParticle{ 100 };

	double cycle{1};
	double amount{1};

	Effect() = default;
	virtual ~Effect() { Clear(); }
	virtual void Update(double dt);
	virtual void Draw(math::TransformMatrix& displayMatrix);
	virtual void MakeParticle() = 0;
	template<typename T>
	void Add();

	
	void SetSize(glm::vec2 s) { minSize = s, maxSize = s; }
	void SetAngle(double a) { minAngle = a, maxAngle = a; }
	void SetSpeed(double s) { minSpeed = s, maxSpeed = s; }
	void SetParticleTime(double pt) { minParticleTime = pt, maxParticleTime = pt; }
	void SetSize(glm::vec2 mins, glm::vec2 maxs) { minSize = mins, maxSize = maxs; }
	void SetAngle(double mina, double maxa) { minAngle = mina, maxAngle = maxa; }
	void SetSpeed(double mins, double maxs) { minSpeed = mins, maxSpeed = maxs; }
	void SetParticleTime(double minpt, double maxpt) { minParticleTime = minpt, maxParticleTime = maxpt; }
	void Clear();
	virtual bool IsEnd() { return effectTime <= effectTimer; } //ÀÌÆåÆ®°¡ ²¨Áö´Â Á¶°Ç
};

class electricEffect : public Effect
{
public:
	electricEffect(math::rect2 startArea);
	~electricEffect(){}
	void MakeParticle() override;
};

class InvisibleEffect : public  Effect
{
public:
	InvisibleEffect(math::rect2 startArea);
	~InvisibleEffect();
	void MakeParticle() override;
};

class ClearEffect : public  Effect
{
public:
	ClearEffect(math::rect2 startArea);
	~ClearEffect();
	void MakeParticle() override;
};
