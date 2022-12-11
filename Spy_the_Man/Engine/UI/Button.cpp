/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "Button.h"
//#include "../../Engine/Poodle.h"
#include "../Engine.h"
#include "Mouse.h"


PM::UI::Button::Button(std::string name, glm::vec2 pos, glm::vec2 size, glm::vec4 color, UILayer layer, bool locked, bool isvalid)
	: UIEntity(name, pos, size, layer, isvalid), color(color), locked(locked)
{
	AddGOComponent(new RectCollision({ {0,0},size }, this));
}


void PM::UI::Button::Update(double dt)
{
	Entity::Update(dt);
}

void PM::UI::Button::Draw(math::TransformMatrix& /*displayMatrix*/) const
{
	glm::vec2 pos = GetPosition();
	glm::vec2 Bsize = GetSize();
	PM::push_settings();
	PM::apply_NDC();
	PM::set_line_color(255, 255, 255, 255);
	PM::set_line_width(5);
	switch (GetButtonState())
	{
	case Normal:
		PM::set_fill_color(color.x, color.y, color.z, color.a);
		break;
	case Pressed: 
		PM::set_fill_color(color.x-30.f, color.y-30.f, color.z-30.f, color.a);
		break;

	case Hovering: 
		PM::set_fill_color(color.x+30.f, color.y+30.f, color.z+30.f, color.a);
		break;

	case Lock: 
		PM::set_fill_color(color.x-255.f, color.y-255.f, color.z-255.f, 200);
		break;
	default: ;
	}
	
	PM::draw_rect(pos.x, pos.y, Bsize.x, Bsize.y);
	PM::pop_settings();
}

void PM::UI::Button::SetSize(glm::vec2 newSize)
{
	if (this->GetSize() != newSize)
	{
		Entity::SetSize(newSize);
		RemoveGOComponent<Collision>();
		AddGOComponent(new RectCollision({ {0,0},newSize }, this));
	}
}


bool PM::UI::Button::IsPressed() const
{
	
	return isValid() && (!locked) && hovering && isPressed;
}

bool PM::UI::Button::IsReleased() const
{
	return isValid() && (!locked) && hovering && isReleased;
}


void PM::UI::Button::Setvalid(bool set)
{
	UIEntity::Setvalid(set);
	if (set == false)
	{
		isPressed = false;
		isReleased = false;
	}
}

PM::UI::ButtonState PM::UI::Button::GetButtonState() const
{
	if(locked == true)
	{
		return Lock;
	}
	if(hovering == true)
	{
		if (Engine::GetInput().LeftMouseIsPressed()) { return Pressed; }
		return Hovering;
	}
		return Normal;
}

void PM::UI::Button::CollisionStay(Entity* mouse)
{
	hovering = true;
	isPressed = Engine::GetInput().LeftMouseIsPressed();
	isReleased = Engine::GetInput().LeftMouseIsReleased();
	dynamic_cast<Mouse*>(mouse)->SetCollided(true);
}

void PM::UI::Button::CollisionExit(Entity* mouse)
{
	hovering = false;
	isPressed = false;
	isReleased = false;
	dynamic_cast<Mouse*>(mouse)->SetCollided(false);
}



PM::UI::ImageButton::ImageButton(std::string n, std::string filepath, glm::vec2 pos, glm::vec2 size, UILayer layer, bool locked, bool valid)
	: Button(n, pos, size, {255,255,255,255}, layer, locked, valid)
{
	texturePtr = Engine::GetTextureManager().Load(filepath);
}

PM::UI::ImageButton::~ImageButton()
{

}

void PM::UI::ImageButton::Draw(math::TransformMatrix& displayMatrix) const
{
	glm::vec2 Bpos = GetPosition();
	glm::vec2 Bsize = GetSize();
	glm::vec4 c = GetStateColor();
	push_settings();
	apply_matrix(displayMatrix.matrix);
	no_outline();
	set_fill_color(c.x, c.y, c.z, c.a);
	draw_rect(Bpos.x, Bpos.y, Bsize.x, Bsize.y);
	texturePtr->Draw(displayMatrix, GetPosition() + glm::vec2{5,5}, GetSize() - glm::vec2{ 10,10 });
	if(GetLock() == true)
	{
		set_fill_color(0, 0, 0, 150);
		draw_rect(Bpos.x, Bpos.y, Bsize.x, Bsize.y);
	}
	pop_settings();
}

PM::UI::HoloImageButton::HoloImageButton(std::string name, std::string filepath, glm::vec2 pos, glm::vec2 size, UILayer layer, bool locked, bool isvalid)
	: ImageButton(name, filepath, pos, size, layer, locked, isvalid)
{
	
}
PM::UI::HoloImageButton::~HoloImageButton()
{
	
}


glm::vec4 PM::UI::HoloImageButton::GetStateColor() const
{
	switch (GetButtonState())
	{
	case Normal:
		return { 39,191,238,150 };
		break;
	case Pressed:
		return { 39,191,238,100 };
		break;

	case Hovering:
		return { 39,191,238,180 };
		break;

	case Lock:
		return { 0,0,0,150 };
		break;
	default:;
	}
	return { 0,0,0,0 };
}


glm::vec4 PM::UI::ImageButton::GetStateColor() const
{
	switch (GetButtonState())
	{
	case Normal:
		return { 255,255,255,100 };
		break;
	case Pressed:
		return { 235,235,235,50 };
		break;

	case Hovering:
		return { 255,255,255,150 };
		break;

	case Lock:
		return { 0,0,0,150 };
		break;
	default:;
	}
	return {0,0,0,0};
}

PM::UI::TextButton::TextButton(std::string name, glm::vec2 pos, glm::vec2 size, std::string text, glm::vec4 color, glm::vec3 tcolor,
	UILayer layer, bool locked, bool isavalid) : Button(name,pos,size,color,layer,locked,isavalid),text(text), tColor(tcolor)
{
	font = new PM::Font(Engine::GetWindow().GetSize().x, Engine::GetWindow().GetSize().y);
	font->Load("fonts/ABeeZee-Regular.ttf", static_cast<unsigned int>(size.y / 6 + size.x / 6));
}

PM::UI::TextButton::~TextButton()
{
	delete font;
}

void PM::UI::TextButton::Update(double dt)
{
	Button::Update(dt);
}

void PM::UI::TextButton::Draw(math::TransformMatrix& display_matrix) const
{
	Button::Draw(display_matrix);
	glm::vec2 Bpos = GetPosition();
	glm::vec2 Bsize = GetSize();
	font->Draw(text, GetCenter().x, GetCenter().y - (Bsize.y / 6 + Bsize.x / 6)/4, 1, false, AlignType::center,tColor);
	if(GetLock() == true)
	{
		push_settings();
		apply_matrix(display_matrix.matrix);
		set_line_color(0, 0, 0);
		PM::set_line_width(5);
		set_fill_color(0, 0, 0, 150);
		draw_rect(Bpos.x, Bpos.y, Bsize.x, Bsize.y);
		
		pop_settings();
	}
}

