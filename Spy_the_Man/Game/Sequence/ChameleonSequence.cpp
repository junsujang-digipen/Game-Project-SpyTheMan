/*--------------------------------------------------------------
File Name: ChemeleonSequence.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 3/15/2021
-----------------------------------------------------------------*/
#include "ChameleonSequence.h"
#include "../Player.h"
#include "../Engine/Engine.h"
#include "../../Engine/SoundManager.h"

ChameleonSequence::ChameleonSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateChameleon);
}

void ChameleonSequence::Update(double dt) {
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void ChameleonSequence::State_Chameleon::Enter(ChameleonSequence*) {
	Engine::GetSoundManager().PlaySound("Chameleon", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
}

void ChameleonSequence::State_Chameleon::Update(ChameleonSequence*, double) {

}

void ChameleonSequence::State_Chameleon::TestForExit(ChameleonSequence* sq) {
	Player* player = static_cast<Player*>(sq->obj1);
	player->SetChameleon(true);
	sq->setIsValid(false);
}