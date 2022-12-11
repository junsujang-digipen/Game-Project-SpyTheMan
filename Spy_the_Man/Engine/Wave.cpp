/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Wave.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 11/29/2021S
-----------------------------------------------------------------*/
#include "Wave.h"
#include "Engine.h"
#include "Poodle.h"

namespace PM {
	Wave::Wave(glm::vec2 pos, float length, float duration, int layer, bool col) : GameObject("wave", pos), length(length), duration(duration), 
		object(nullptr), layer(layer), col(col) {
		Engine::GetLogger().LogDebug("Wave Maded");
		Engine::GetGSComponent<PM::World>()->Add(this, layer);
		if (col == true) {
			this->AddGOComponent(new PM::CircleCollision(0, this));
		}
	}

	Wave::Wave(GameObject* object, float length, float duration, int layer, bool col) : GameObject("wave", object->GetPosition()), object(object), 
		length(length), duration(duration), layer(layer), col(col) {
		Engine::GetLogger().LogDebug("Wave Maded");
		Engine::GetGSComponent<PM::World>()->Add(this, layer);
		if (col == true) {
			this->AddGOComponent(new PM::CircleCollision(0, this));
		}
	}

	void Wave::Update(double dt) {
		if (object != nullptr) {
			SetPosition({ object->GetPosition().x + object->GetSize().x / 2, object->GetPosition().y });
		}
		time += static_cast<float>(dt);
		if (static_cast<int>(floor(time * 2)) != lasttime && newwave == false && time < duration) {
			newwave = true;
			if (object != nullptr) {
				new Wave(object, length, duration - time, layer, false);
			}
			else {
				new Wave(GetPosition(), length, duration - time, layer, false);
			}
		}
		float sizeAdd = static_cast<float>(length * 50);
		if (time >= duration || wavesize >= sizeAdd) {
			dying = true;
		}
		if (dying == true) {
			alpha -= 3;
			dyingtime -= static_cast<float>(dt);
			sizeAdd -= wavesize / 2;
		}
		if (col == true) {
			this->GetGOComponent<PM::CircleCollision>()->SetCollision(wavesize * 5);
			if (time >= duration + 0.4f) {
				dead = true;
			}
		}
		else {
			if (dyingtime <= 0) {
				dead = true;
			}
		}
		lasttime = static_cast<int>(floor(time * 2));
		wavesize += sizeAdd * static_cast<float>(dt);
		if (dead == true) {
			Destroy();
		}
	}

	void Wave::Draw(math::TransformMatrix& cameraMatrix) const {
		push_settings();
		set_line_width(5);
		set_line_color(255, 0, 0, alpha);
		no_fill();
		apply_matrix(cameraMatrix.matrix);
		draw_ellipse(GetPosition().x, GetPosition().y, wavesize, wavesize);
		pop_settings();
		GameObject::Draw(cameraMatrix);
	}

	bool Wave::CanCollideWith(EntityType ) {
		/*switch (objectBType) {
		case EntityType::Basic_NPC: [[fallthrough]];
		case EntityType::Enemy_NPC: [[fallthrough]];
		case EntityType::Worker_NPC: [[fallthrough]];
		case EntityType::Target_NPC:
			return true;
			break;
		}*/
		return false;
	}
}