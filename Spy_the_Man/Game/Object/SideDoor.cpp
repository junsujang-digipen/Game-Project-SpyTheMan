/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "SideDoor.h"


#include "ButtonManager.h"
#include "../../Engine/Engine.h"
#include "../Sequence/OpenDoorSequence.h"

#include "../UI/SkillButton.h"
#include "../Player.h"
#include "../Engine/Wave.h"
SideDoor::SideDoor(std::string name, glm::vec2 pos, bool locked) : GameObject(name, pos), lock(locked)
{
	SetSize(SIDEDOORSIZE);
	drawRect = GetSize();
	
	GetGOComponent<ButtonManager>()->AddButton(new SequenceButton<OpenDoorSequence>(
		"OpenDoor", this, "assets/GameObject/UI/Door.png"));
	
		GetGOComponent<ButtonManager>()->GetButton("OpenDoor")->SetLock(locked);
		SetCollision(true);

}
SideDoor::~SideDoor()
{
}
void SideDoor::Update(double dt)
{
	GameObject::Update(dt);
	PM::Collision* col = GetGOComponent<PM::Collision>();
	if (lock == false)
	{
		if (col != nullptr)
		{
			Player* player = dynamic_cast<Player*>(Engine::GetGSComponent<PM::World>()->GetPlayerPtr());
			if (player->GetCheckRun() == true && col->DoesCollideWith(player))
				{
					Engine::GetSoundManager().PlaySound("doorOpen", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
					OpenAndClose();
					new PM::Wave(GetCenter(), 5, 1.5);
				}
		}
	}
	PM::World* world = Engine::GetGSComponent<PM::World>();
	Player* playerptr = dynamic_cast<Player*>(world->GetPlayerPtr());
	if (playerptr->IsVent() == true) {
		GetGOComponent<ButtonManager>()->GetButton("OpenDoor")->SetLock(true);
	}
	else {
		GetGOComponent<ButtonManager>()->GetButton("OpenDoor")->SetLock(false);
	}
}
void SideDoor::UnLock()
{
	GetGOComponent<ButtonManager>()->GetButton("OpenDoor")->SetLock(false);
	lock = false;
}




void SideDoor::Draw(math::TransformMatrix& displayMatrix) const
{
		PM::push_settings();
		PM::set_line_color(0, 0, 0);
		PM::no_outline();
		glm::vec2 Dposition = GetPosition();
		glm::vec2 Dsize = drawRect;
		
		PM::apply_matrix(displayMatrix.matrix);
		if (isOpen == false)
		{
			PM::set_fill_color(doorColor.x, doorColor.y, doorColor.z, doorColor.w);
			PM::draw_rect(Dposition.x, Dposition.y, Dsize.x, Dsize.y);
		}
		else
		{
			PM::set_fill_color(doorColor.x, doorColor.y, doorColor.z, 100);
			PM::draw_rect(Dposition.x, Dposition.y, Dsize.x * 3, Dsize.y);
		}
    	PM::pop_settings();
		GameObject::Draw(displayMatrix);
}
void SideDoor::SetCollision(bool set)
{
	if(set)
	{
		if (GetGOComponent<PM::RectCollision>() == nullptr)
		{
			AddGOComponent(new PM::RectCollision(Door_objPositionSize, this));
		}
	}
	else
	{
		if (GetGOComponent<PM::RectCollision>() != nullptr)
		{
			RemoveGOComponent<PM::RectCollision>();
		}
	}
}

void  SideDoor::OpenAndClose()
{
	
	if(isOpen)
	{
		//Engine::GetSoundManager().PlaySound("doorOpen", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
		isOpen = false;
	}
	else
	{
		//Engine::GetSoundManager().PlaySound("doorOpen", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
		isOpen = true;
	}
	SetCollision(!isOpen);
}
