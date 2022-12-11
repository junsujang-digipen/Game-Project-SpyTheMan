#include "Phone.h"

#include <iomanip>

#include "../../Engine/Engine.h"
#include "../../Engine/World.h"
#include "../../Engine/UI/UIEntity.h"
#include "../../Engine/UI/Button.h"
#include "../../Engine/UI/Image.h"
#include "../../Engine/UI/Text.h"
#include "../../Engine/Helper_func_map.h" 
#include "../Game/Object/Action/ActionManager.h"
#include "../Game/UI/SkillButton.h"
#include "../../Engine/Declaration.h"
Phone::Phone() : Component("Phone")
{


	settingButton = new PM::UI::ImageButton("Setting Button", "assets/GameObject/UI/gear.png", { 0,0 }, { 0,0 }, UILayer::PHONEUI);
	//time = new PM::UI::Text("", "time", { 0,0 }, 30, PM::AlignType::left, UILayer::PHONEUI);
	SetSkill();
}

Phone::~Phone()
{
	skillList.clear();
}


void Phone::Update(double dt)
{
	if (getIsValid() == true)
	{
	}
	//std::setprecision(4);
	//std::string istr = to_string(static_cast<int>(timer));
	//time->SetText(istr + to_string(timer).at(istr.size()) + to_string(timer).at(istr.size()+1));
	//timer += dt;
}

void Phone::TurnOn(bool )
{

}

void Phone::AddSkill(PM::UI::Button* Button)
{
	skillList.push_back(Button);
	int i = 0;
	glm::vec2 wSize = Engine::GetWindow().GetSize();
	for (auto sl : skillList)
	{
		
		sl->SetSize({ 70,70 });
		sl->SetPosition({ wSize.x - 550 + (100 * (i%5)),50  + (100 * (i / 5)) });
		i++;
	}
}

void Phone::setIsValid(bool set)
{
	settingButton->Setvalid(set);
	for (auto b : skillList)
	{
		b->Setvalid(set);
	}
	Engine::GetGSComponent<PM::World>()->SetdtMagnification(1.f);
	Component::setIsValid(set);
}



void Phone::SetSkill()
{
	std::list<Act::Action*> acts  = Engine::GetGSComponent<ActionManager>()->GetAllAction();

	for(Act::Action* act : acts)
	{
		if(act->GetSkillType() != SkillType::Null)
		{
			AddSkill(new SkillButton(act,Engine::GetGSComponent<PM::World>()->GetPlayerPtr(),act->GetSkillData()->imagePath,UILayer::PHONEUI) );
		}
	}
}
