#include "ValidManager.h"
#include "../Engine/Engine.h"

#include "UI/Phone.h"
#include "../Game/Object/ButtonManager.h"
void ValidManager::SetPhone(bool set)
{
	Engine::GetGSComponent<Phone>()->setIsValid(set);
}



void ValidManager::SetButton(bool set)
{
	Engine::GetGSComponent<PM::World>()->SetCanUseButton(set);
}

void ValidManager::SetLockInput(bool set)
{
	Engine::GetInput().SetLockInput(set);
}

void ValidManager::Setdt(double magnification)
{
	Engine::GetGSComponent<PM::World>()->SetdtMagnification(magnification);
}
