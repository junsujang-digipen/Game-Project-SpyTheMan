/*--------------------------------------------------------------
File Name: Sequence_base.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/9/2021
-----------------------------------------------------------------*/
#include "Sequence_base.h"
#include "../Engine.h"
#include "SequenceManager.h"
namespace PM {

	Sequence_base::Sequence_base(SequenceType type, GameObject* obj1, GameObject* obj2) :type(type),obj1(obj1),obj2(obj2) {
		Engine::GetGSComponent<SequenceManager>()->AddSequence(this);
	}
	Sequence_base::~Sequence_base()
	{
		objInit();
		
	}
	void Sequence_base::objInit()
	{
		if (obj1 != nullptr) {
			obj1->putSequence(nullptr);
		}
		if (obj2 != nullptr) {
			obj2->putSequence(nullptr);
		}
		obj1 = nullptr;
		obj2 = nullptr;
	}
}