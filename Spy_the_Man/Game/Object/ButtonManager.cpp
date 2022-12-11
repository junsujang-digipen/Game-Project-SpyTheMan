#include "ButtonManager.h"


#include "../../Engine/Poodle.h"
#include "../Engine/GameObject.h"
#include "../Engine/UI/Button.h"


ButtonManager::ButtonManager(PM::GameObject* obj) : objPtr(obj)	{}

ButtonManager::~ButtonManager() {}


void ButtonManager::Update(double dt)
{

	if (activate == true)
	{
		int bSize = static_cast<int>(buttons_.size());
		for (int i = 0; i < bSize; i++)
		{
			glm::vec2 center = objPtr->GetCenter();
			glm::fvec2 objCenterPos = PM::WorldPosToDevicePos(center);
			PM::UI::Button* button = buttons_[i];
			buttons_[i]->SetSize({ 70,70 });
			button->Update(dt);
			glm::vec2 boxSize = button->GetSize();
			if (bSize == 1)
			{
				button->SetPosition({ objCenterPos.x - boxSize.x / 2,objCenterPos.y - boxSize.y / 2 });
			}
			else if (bSize > 1)
			{
				glm::dvec2 pos = { objCenterPos.x + (cos(6.28 / (float)(i + 1)) * boxSize.x / 1.5) - boxSize.x/2,objCenterPos.y + sin(6.28 / (i+1)) * boxSize.y / 1.5 - boxSize.y / 2 };
				button->SetPosition(pos);
			}
		}
		activate = false;
	}
	else if(check == true && activate == false)
	{
		Activate(false);
	}
}

void ButtonManager::AddButton(PM::UI::Button* button)
{
	buttons_.push_back(button);
}


PM::UI::Button* ButtonManager::GetButton(std::string n)
{
	for (PM::UI::Button* b : buttons_)
	{
		if (b->GetName() == n)
		{
			return b;
		}
	}
	return nullptr;
}


void ButtonManager::RemoveButton(std::string n)
{
	size_t size = buttons_.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (buttons_[i]->GetName() == n)
		{
			buttons_.erase(buttons_.begin() + i);
			return;
		}
	}
}


void ButtonManager::Activate(bool active)
{
	activate = active;
	check = active;
	for (auto& button : buttons_)
	{
		button->Setvalid(active);
	}
}
