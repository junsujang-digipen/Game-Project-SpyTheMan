/*--------------------------------------------------------------
File Name: SequenceManager.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 10/27/2021
-----------------------------------------------------------------*/
#include "SequenceManager.h"
#include "Sequence_base.h"
#include "../GameObject.h"
namespace PM {

	SequenceManager::~SequenceManager()
	{
		playSequence.clear();
		for (auto& sq:sequenceQue) {
			delete sq;
		}
		sequenceQue.clear();
	}

	void SequenceManager::AddSequence(Sequence_base* sqc)
	{
		sequenceQue.push_back(sqc);
	}

	void SequenceManager::Update(double dt)//plan
	{
		for (auto& SQ : sequenceQue) {
			switch (SQ->getType())
			{
			case SequenceType::Player:
				if (playerSequence == SQ) {
					break;
				}
				if (playerSequence != nullptr) {
					playerSequence->setIsValid(false);
					playerSequence->objInit();
				}
				playerSequence = SQ;
				SQ->obj1->putSequence(SQ);
				SQ->obj2->putSequence(SQ);
				break;
			case SequenceType::Other:
				if (SQ->obj1->GetNowSequence() == nullptr
					&& SQ->obj2->GetNowSequence() == nullptr) {
					SQ->obj1->putSequence(SQ);
					SQ->obj2->putSequence(SQ);
					playSequence.push_back(SQ);
				}
				break;
			default:
				break;
			}
		}


		for (auto& PSQ : playSequence) {
			PSQ->Update(dt);
		}
		if (playerSequence != nullptr) {
			playerSequence->Update(dt);
			if (playerSequence->getIsValid() == false) {
				playerSequence = nullptr;
			}
		}

		playSequence.remove_if([&](Sequence_base* sq)->bool {
			return sq->getIsValid() == false;
			});
		sequenceQue.remove_if([&](Sequence_base* sq)->bool {
			if (sq->getIsValid() == false) {
				
				delete sq;
				return true;
			}
			return false;});
	}
}