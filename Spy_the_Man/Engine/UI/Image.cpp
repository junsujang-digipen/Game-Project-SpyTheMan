/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "Image.h"

#include "../Engine.h"

PM::UI::Image::Image(std::string name, std::string FilePath, glm::vec2 pos, glm::vec2 size, UILayer layer) : UIEntity(name, pos, size, layer)
{
	texturePtr = Engine::GetTextureManager().Load(FilePath);
}

void PM::UI::Image::Draw(math::TransformMatrix& displayMatrix) const
{
	glm::vec2 pos = GetPosition();
	glm::vec2 Isize = GetSize();
	texturePtr->Draw(displayMatrix, pos, Isize);
}

PM::UI::Rect::Rect(std::string name, glm::vec2 pos, glm::vec2 size, UILayer layer,
	glm::vec4 color) : UIEntity(name,pos,size,layer), color(color)
{
}

void PM::UI::Rect::Draw(math::TransformMatrix&) const
{
	glm::vec2 pos = GetPosition();
	glm::vec2 Isize = GetSize();
	PM::push_settings();
	PM::apply_NDC();
	PM::set_fill_color(color.r, color.g, color.b, color.a);
	PM::draw_rect(pos.x, pos.y, Isize.x, Isize.y);
	PM::pop_settings();
}

void PM::UI::holo::Draw(math::TransformMatrix& ) const
{
	glm::vec2 pos = GetPosition();
	glm::vec2 Hsize = GetSize();
	PM::push_settings();
	PM::apply_NDC();
	PM::no_outline();
	glm::vec4 Hcolor = GetColor();
	PM::set_fill_color(Hcolor.r, Hcolor.g, Hcolor.b,Hcolor.a);
	PM::use_shader("Hologram");
	PM::draw_rect(pos.x, pos.y, Hsize.x, Hsize.y);
	PM::pop_settings();
}
