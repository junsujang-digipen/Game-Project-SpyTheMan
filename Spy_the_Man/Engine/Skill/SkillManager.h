/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SkillManager.h
Project: GAM200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#pragma once


#include <iostream>
#include <map>
#include <vector>
#include "../../Game/SkillType.h"


using namespace std;
namespace PM
{
	enum class SequenceType;
	class GameObject;

	class SkillManager
	{
	public:
		struct SkillData {
			SkillData(string name, HowUse how, WhichType which, int pointneed, int use = 0, float cooldown = 0, std::string imagePath = "assets/UI/Skill/Empty.png");
			string name;
			string howuse;
			string whichtype;
			string imagePath;
			HowUse howuseE;
			WhichType whichtypeE;
			int pointneed = 0;
			int use = 0;
			float cooldown;
			bool valid = false;
		};

		~SkillManager();
		void Add(SkillType skilltype, SkillData* skill);
		bool GetValid(SkillType skilltype);
		bool GetNotYet() { return notyet; }
		bool GetNoPoint() { return nopoint; }
		void SetNotYet(bool n) { notyet = n; }
		void SetNoPoint(bool n) { nopoint = n; }
		void UnLockSkill(SkillType skilltype);
		void LoadGS();
		int GetSkillPoint() { return skillpoint; }
		void AddSkillPoint(int add) { skillpoint += add; }
		int GetUsedPoint() { return usedpoint; }
		SkillData* GetSkillData(SkillType skilltype);
		vector<SkillType> &CurrSkills() { return currskills; }

	private:
		map<SkillType, SkillData*> data;
		vector<SkillType> currskills;
		
		int skillpoint = 15;
		int usedpoint = 0;
		bool notyet = false;
		bool nopoint = false;
	};
}