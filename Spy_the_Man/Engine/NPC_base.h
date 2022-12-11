/*--------------------------------------------------------------
File Name: NPC_base.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/5/2021
-----------------------------------------------------------------*/
#pragma once

#include "GameObject.h"
#include <list>

namespace PM {
	class Texture;
}

namespace PM {
	class NPC_base :public GameObject{
	protected:
		PM::GameObject* player{nullptr};
		double speed{ 325. };

	public:
		using GameObject::GameObject;
		PM::GameObject* getPlayerPtr() { return player; }

		double getSpeed() { return speed; }
		double getRunSpeed() { return speed*2; }
		void setSpeed(double sp = 325.) { speed = sp; }
		void updateSpeed(double val) { speed += val; }

		~NPC_base()override {}
	};
}

