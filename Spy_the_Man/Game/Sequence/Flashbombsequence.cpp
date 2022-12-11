/*--------------------------------------------------------------
File Name: FlashbombSequence.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/24/2021
-----------------------------------------------------------------*/
#include "FlashbombSequence.h"
#include "../Player.h"
#include "../Engine/Engine.h"
#include "../Object/Action/ActionManager.h"
#include "../../Engine/SoundManager.h"

FlashbombSequence::FlashbombSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateFlashbomb);
}

void FlashbombSequence::Update(double dt) {
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void FlashbombSequence::State_Flashbomb::Enter(FlashbombSequence* sq) {
	Player* player = static_cast<Player*>(sq->obj1);
	Engine::GetSoundManager().PlaySound("FlashBomb", player->GetPosition());
	sq->flashbombptr = new Flashbomb({ player->GetCenter().x, player->GetPosition().y });
	Engine::GetGSComponent<PM::World>()->Add(sq->flashbombptr);
}

void FlashbombSequence::State_Flashbomb::Update(FlashbombSequence*, double) {

}

void FlashbombSequence::State_Flashbomb::TestForExit(FlashbombSequence* sq) {
	Engine::GetGSComponent<ActionManager>()->GetAction(SkillType::Flashbomb)->SetTimer(0);
	sq->setIsValid(false);
}