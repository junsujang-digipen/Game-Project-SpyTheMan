/*--------------------------------------------------------------
File Name: Lazer.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 12/7/2021
-----------------------------------------------------------------*/
#pragma once
#include "../Engine/GameObject.h"

class Lazer : public PM::GameObject
{
public:
	Lazer(std::string name, glm::vec2 pos, glm::vec2 siz = { 100,50}, double onOffTime = 0., glm::vec4 xMovable = {});
	~Lazer();

	void Update(double dt) override;
	void Draw(math::TransformMatrix& displayMatrix) const override;
	void SetPowerOff(bool tf) { isPowerOff = tf; }
	void SetOnOffInverse() { isOn = !isOn; }
	EntityType GetEntityType() const override;
	std::string GetEntityTypeName() const override;

private:
	bool isPowerOff{false};
	bool isOn{true};

	const int LazerSize{20};
	glm::vec4 xMovable{};
	const double onOffTime{};
	double nowT{};
};
