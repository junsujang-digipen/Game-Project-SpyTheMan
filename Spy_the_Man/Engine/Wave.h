#pragma once
/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Wave.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 11/29/2021
-----------------------------------------------------------------*/
#include <glm/glm.hpp>
#include <list>
#include "Component.h"
#include "TransformMatrix.h"
#include "Entity.h"
#include "GameObject.h"

namespace PM {
	class Wave : public GameObject {
	public:
		Wave(glm::vec2 pos, float length, float duration, int layer = 1, bool col = true);
		Wave(GameObject* object, float length, float duration, int layer = 1, bool col = true);
		void Update(double dt) override;
		void Draw(math::TransformMatrix& cameraMatrix) const override;
		bool CanCollideWith(EntityType objectBType) override;
		EntityType GetEntityType() const override { return EntityType::Wave; }
		std::string GetEntityTypeName() const override { return "Wave"; }
		void SetDying();
		float GetRadius();
		float GetLength();
		bool GetDead();
		bool GetDying();

	private:
		float time = 0;
		float dyingtime = 1.0f;
		float wavesize = 0;
		int lasttime = 0;
		int count = 1;
		double alpha = 150;

		float length;
		float duration;
		GameObject* object;
		int layer;
		bool col;

		bool newwave = false;
		bool dead = false;
		bool dying = false;
	};
}