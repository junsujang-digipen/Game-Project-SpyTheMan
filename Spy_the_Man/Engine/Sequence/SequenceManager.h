#pragma once
/*--------------------------------------------------------------
File Name: SequenceManager.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 10/27/2021
-----------------------------------------------------------------*/
#include "../Component.h"
#include <list>

namespace PM {
	class Sequence_base;

	class SequenceManager :public PM::Component {//gs component

		std::list<Sequence_base*> sequenceQue{};
		std::list<Sequence_base*> playSequence{};
		Sequence_base* playerSequence{};
	public:
		~SequenceManager();
		void AddSequence(Sequence_base* sqc);
		void Update(double dt);
	};
}