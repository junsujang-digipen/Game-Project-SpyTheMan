/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SkillBase.h
Project: GAM200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#pragma once
#include <iostream>
#include <map>
#include <string>

#include "../../Engine/Engine.h"
#include "../../Game/SkillType.h"
#include "../../Engine/Sequence/Sequence_base.h"
#include "../../Engine/Component.h"

namespace PM
{

	//class SkillManager;
	class SkillBase  {

	public:
		SkillBase(SkillType skilltype);
		virtual void Load() = 0;
		virtual void doing(PM::SequenceType , PM::GameObject* , PM::GameObject* ) = 0;
		virtual void SetValid() = 0;
		virtual bool GetValid() = 0;
		virtual std::string getName() = 0;
	};
}
