/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include "UIEntity.h"


namespace PM
{
	namespace UI
	{
		class Mouse : public UIEntity
		{
		public:
			Mouse();
			void Update(double dt) override;
			void Draw(math::TransformMatrix& displayMatrix) const override;
			bool CanCollideWith(EntityType type) override { return type != EntityType::UI; }
			void SetCollidedObj(UIEntity* entity) { collidedObj = entity; }
			UIEntity* GetCollidedObj() { return collidedObj; }
		private:
			UIEntity* collidedObj;
		};
	}
}
