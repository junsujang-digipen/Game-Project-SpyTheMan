/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include "UIEntity.h"

namespace PM {
	class Texture;
	namespace UI
	{

		class Image : public UIEntity
		{
		private:
			PM::Texture* texturePtr;
		public:
			Image(std::string name, std::string FilePath, glm::vec2 pos, glm::vec2 size, UILayer layer = UILayer::DEFAULT);
			void Draw(math::TransformMatrix& displayMatrix) const override;
		};

		class Rect : public UIEntity
		{
		private:
			glm::vec4 color{ 255,255,255,255 };
		public:
			Rect(std::string name, glm::vec2 pos, glm::vec2 size, UILayer layer = UILayer::DEFAULT, glm::vec4 color = {255 * 0.3961,255 * 0.8078,255,255});
			glm::vec4 GetColor() const { return color; }
			void SetColor(glm::vec4 newColor) { color = newColor; }
			virtual void Draw(math::TransformMatrix& displayMatrix) const override;
		};

		class holo : public  PM::UI::Rect
		{
		public:
			holo(std::string name, glm::vec2 pos, glm::vec2 size, UILayer layer = UILayer::DEFAULT, glm::vec4 color = { 255,255,255,255 })
				: Rect(name, pos, size, layer, color)
			{}
			void Draw(math::TransformMatrix& displayMatrix) const override;
		};
	}
}