#include "ActivateVision.h"

#include "../../Engine/Engine.h"
#include "../../Engine/World.h"
#include "../Engine/GameObject.h"


ActivateVision::ActivateVision(PM::GameObject* thisObject)
	:Component("ActivateVision"), object(thisObject)
{}


void ActivateVision::Update(double)
{
	if (getIsValid() == true)
	{
		Engine::GetGSComponent<PM::World>()->SetRoomVision(object);
	}
}
