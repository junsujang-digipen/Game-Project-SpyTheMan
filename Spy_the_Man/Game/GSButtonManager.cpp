#include "GSButtonManager.h"
#include "../Engine/Engine.h"
#include "Object/ButtonManager.h"
#include "Object/Room.h"
#include "../Engine/World.h"

void GSButtonManager::Update(double)
{
	
	PM::World* world = Engine::GetGSComponent<PM::World>();
	if (world->GetCanUseButton() == true)
	{
		PM::Room* b = world->GetRoom(world->GetPlayerPtr());

		if (b->GetLeftObj() != nullptr)
		{
			b->GetLeftObj()->GetGOComponent<ButtonManager>()->Activate(true);
		}
		if (b->GetRightObj() != nullptr)
		{
			b->GetRightObj()->GetGOComponent<ButtonManager>()->Activate(true);
		}
		for (auto& obj : b->Objects())
		{
			obj->GetGOComponent<ButtonManager>()->Activate(true);
		}
	}
}
