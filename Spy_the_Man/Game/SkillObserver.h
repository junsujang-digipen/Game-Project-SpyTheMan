#pragma once
#include "../Engine/Button.h"
#include "SkillType.h"
class SkillObserver
{
public:
	virtual ~SkillObserver() { }
	virtual void onNotify(SkillType skillType) = 0;
};
