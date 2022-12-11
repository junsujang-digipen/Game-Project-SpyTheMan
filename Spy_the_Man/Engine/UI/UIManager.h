/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <list>
#include <map>
#include <string>
#include <vector>

#include "../Component.h"
#include "../Declaration.h"
namespace PM {
	namespace UI
	{
		class WorldMouse;
		class Mouse;
		class UIEntity;


		class UIManager : public Component
		{
		private:
			friend class UIEntity;//For AddUI
			Mouse* mouse{ nullptr };

			//std::list<UIEntity*> Interfaces;
			std::multimap<int, UIEntity*> ValidInterfaces;
			std::multimap<int, UIEntity*> Interfaces;
			std::vector<std::pair<int,UIEntity*>> destroyInterfaces;
			void AddUI(UIEntity* ui, UILayer layer = UILayer::DEFAULT);

		public:
			UIManager() : Component("UIManager") { Load(); }
			virtual ~UIManager() { Unload(); }

			virtual void Update(double dt) override;
			void Draw();
			void CollideTest();
			void Load();
			void Unload();
			void UnloadUI();
			Mouse* GetMouse() { return mouse; }

			//void SetUIValid(UIEntity* entity, bool set);
			static glm::vec2 sequenceButtonSize;
		};
	}
}
