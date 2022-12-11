#include "HelpText.h"


#include "../../Engine/Engine.h"
#include "../../Engine/Font.h"
#include "../Engine/UI/Image.h"
#include "../Engine/UI/Text.h"
#include "../Engine/UI/Mouse.h"
HelpText::HelpText() : Component("HelpText")
{
	background = new PM::UI::holo("HelpTextBackground", { 0,20 }, { 800,100 }, UILayer::PHONEUI, HOLOGRAMDEEPCOLOR);
	text = new PM::UI::Text("", "HelpText", { 30,60 }, 40.f, PM::AlignType::left, UILayer::PHONEUI,{255,255,255});
}


HelpText::~HelpText()
{
	
}

void HelpText::Update(double)
{
	if (getIsValid() == true)
	{
		PM::UI::UIEntity* entity = Engine::GetGSComponent<PM::UI::UIManager>()->GetMouse()->GetCollidedObj();
		if (entity != nullptr)
		{
			std::string txt = entity->GetHelpText();
			if (txt != "")
			{
				background->SetColor(HOLOGRAMDEEPCOLOR);
				text->SetText(txt);
			}
		}
		else
		{
			background->SetColor(HOLOGRAMCOLOR);
			text->SetText("");
		}
	}
}

void HelpText::setIsValid(bool tf)
{
	background->Setvalid(tf);
	text->Setvalid(tf);
}
