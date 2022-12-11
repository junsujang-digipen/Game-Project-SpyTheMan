/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "../Engine.h"
#include "../Font.h"
#include "Text.h"

PM::UI::Text::Text(std::string t, std::string n, glm::vec2 pos, float size, AlignType type, UILayer layer ,glm::vec3 c)
	: UIEntity(n, pos, { 0,0 }, layer, true), color(c),text(t),alignType(type)
{
	font = new PM::Font(Engine::GetWindow().GetSize().x, Engine::GetWindow().GetSize().y);
	font->Load("fonts/ABeeZee-Regular.ttf", static_cast<unsigned int>(size));
}

PM::UI::Text::~Text()
{
	delete font;
}



void PM::UI::Text::Draw(math::TransformMatrix&) const
{
	glm::vec2 pos = GetPosition();
	font->Draw(text, pos.x, pos.y, 1, false, alignType, color);
}

void PM::UI::Text::AddString(std::string t)
{
	text += t;
}

void PM::UI::Text::SetText(std::string t)
{
	text = t;
}

void PM::UI::Text::SetAlign(AlignType align)
{
	alignType = align;
}

void PM::UI::Text::SetSize(float size_)
{
	font->Load("fonts/ABeeZee-Regular.ttf", static_cast<unsigned int>(size_));
}
