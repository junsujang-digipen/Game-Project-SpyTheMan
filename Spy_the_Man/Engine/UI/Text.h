/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <string>

#include "UIEntity.h"


namespace PM
{
	class Font;
	namespace UI
	{
		class Text : public UIEntity
		{
		private:
			std::string text;
			PM::Font* font;
			AlignType alignType;
			glm::vec3 color{ 0,0,0 };
		public:
			Text(std::string text, std::string name, glm::vec2 pos, float size, AlignType type = AlignType::left, UILayer layer = UILayer::DEFAULT, glm::vec3 color = { 0,0,0 });
			~Text();
			void Draw(math::TransformMatrix& displayMatrix) const override;
			void SetText(std::string text);
			void AddString(std::string text);
			void SetAlign(AlignType align);
			void SetSize(float size);
		};
	}
}
