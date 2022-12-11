/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include "../Entity.h"

namespace PM {
	namespace UI
	{
		class Mouse;

		class UIEntity : public Entity
		{
		public:
			UIEntity(std::string name, UILayer layer = UILayer::DEFAULT);
			UIEntity(std::string name, glm::vec2 pos, glm::vec2 size, UILayer layer = UILayer::DEFAULT, bool valid = true);
			virtual ~UIEntity() {}
			bool CanCollideWith(EntityType) override { return true; } //it will interact with only mouse
			virtual void Setvalid(bool v);
			bool isValid() const { return valid; }
			UILayer GetLayer() const { return layer; }
			EntityType GetEntityType() const override { return EntityType::UI; }
			std::string GetEntityTypeName() const override { return "UI"; }
			void SetHelpText(std::string txt) { htxt = txt; }
			virtual std::string GetHelpText() { return htxt; }
			
			void UpdateCollision(Mouse* mouse);
			bool GetCollided() { return isCollided; }
			void SetCollided(bool collided) { isCollided = collided; }

		private:
			UILayer layer;
			std::string htxt{ "" };
			bool valid;
		
			bool isCollided{ false };

			
		};
	}
}
