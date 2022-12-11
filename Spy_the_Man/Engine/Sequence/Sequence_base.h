#pragma once
/*--------------------------------------------------------------
File Name: Sequence_base.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 10/27/2021
-----------------------------------------------------------------*/
#include "../Component.h"
namespace PM {
	class GameObject;
	class SequenceManager;
	enum class SequenceType
	{
		Player = 0,
		Other,
		Nothing
	};

	class Sequence_base :public Component{//component?
		friend SequenceManager;
		
	protected:
		GameObject* obj1{ nullptr };
		GameObject* obj2{ nullptr };
		
		SequenceType type{};

	public:
		Sequence_base(SequenceType type, GameObject*, GameObject*);
		~Sequence_base();
		SequenceType getType() { return type; }
		virtual void objInit();
	};
}