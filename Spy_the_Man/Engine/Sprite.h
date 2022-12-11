#pragma once
/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.h
Purpose: This is the service provider for our games engine services
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 10/27/2021
-----------------------------------------------------------------*/
#include "component.h"
#include <glm/glm.hpp>
#include <vector>
#include "Entity.h"

namespace math {
	class TransformMatrix;
}

namespace PM {
	class Texture;
	class Animation;
	class GameObject;
}

namespace PM {
	class Sprite : public Component {
	public:
		Sprite(std::string spriteInfoFile, EntityType entitytype = EntityType::Empty);
		~Sprite();
		void Load(std::string spriteInfoFile);
		void Update(double dt) override;
		void Draw(math::TransformMatrix displayMatrix, glm::vec2 position = { -1, -1 }, glm::vec2 size = { 2, 2 });
		void PlayAnimation(int anim);
		void SetType(EntityType entitytype);
		void SetTrans(bool trans) { transparent = trans; }
		void SetCloset(bool clo) { closet = clo; }
		bool GetTrans() { return transparent; }
		int GetCurrentAnim();
		bool IsAnimationDone();
		glm::vec2 GetFrameSize() const;
	private:
		glm::vec2 GetFrameTexel(int frameNum) const;
		Texture* texturePtr;
		int currAnim;
		glm::vec2 frameSize;
		bool transparent;
		bool closet;
		std::vector<glm::vec2> frameTexel;
		std::vector<Animation*> animations;
		std::vector<glm::vec2> hotSpotList;
		EntityType type;
	};
}