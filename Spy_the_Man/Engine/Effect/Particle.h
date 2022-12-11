/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <glm/glm.hpp>
#include <random>

namespace math
{
	class TransformMatrix;
}

namespace DOG {
	class Camera;
}

struct Particle
{
	glm::dvec2 position{ 0 };
	glm::dvec2 velocity{ 0 };
	glm::dvec4 color{255,255,255,255};
	glm::dvec2 size{ 10,10 };
	double speed{ 0 };
	double angle{ 0 };
	double time{ 1 };
	double timer{ 0 };
	bool isDead = false;
	static constexpr double gravity = 128;

	Particle(glm::vec2 Position, double Angle, double time, double Speed, glm::vec2 size);
	virtual ~Particle() {}

	virtual void Update(double dt);
	virtual void Draw(math::TransformMatrix& displayMatrix);

	void SetPosition(glm::vec2 pos);
	void SetVelocity(glm::vec2 vel);
	void SetSize(glm::vec2 siz) { size = siz; }
	void SetSpeed(double speed);
	void SetAngle(double angle);

	glm::vec2 GetPosition() { return position; }
	glm::vec2 GetVelocity() { return velocity; }
	glm::vec2 GetSize() { return size; }
	double GetSpeed() { return speed; }
	double GetAngle() { return angle; }
	bool IsDead() { return isDead; }
};

struct electricParticle : public Particle
{
	static constexpr glm::vec4 yellow{ 255,255,0,205 };

	electricParticle(glm::vec2 Position, double Angle, double Time, double Speed, glm::vec2 size);
	~electricParticle() {}
	void Update(double dt) override;
	//void Draw(math::TransformMatrix& displayMatrix) override;
};

struct InvisibleParticle : public Particle
{
	InvisibleParticle(glm::vec2 Position, double Angle, double Time, double Speed, glm::vec2 size);
	~InvisibleParticle() {}
	void Update(double dt) override;
	void Draw(math::TransformMatrix& displayMatrix) override;

};

struct ClearParticle : public Particle
{
	ClearParticle(glm::vec2 Position, double Angle, double Time, double Speed, glm::vec2 size);
	~ClearParticle() {}
	void Update(double dt) override;
	void Draw(math::TransformMatrix& displayMatrix) override;

};