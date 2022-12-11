#pragma once
/*--------------------------------------------------------------
File Name: Sequence_example.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 10/27/2021
-----------------------------------------------------------------*/
#include "../../Engine/Sequence/Sequence_base.h"


class Sequence_example:public PM::Sequence_base {

	// 시퀀스에 필요한 오브젝트들 (포인터)
	// 필요한 변수 추가a
public:
	Sequence_example(PM::SequenceType type) :Sequence_base(type,nullptr,nullptr) {}
	void Update(double dt)override;
};