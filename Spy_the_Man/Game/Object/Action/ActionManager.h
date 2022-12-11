#pragma once
#include <list>


#include "Action.h"
#include "../../../Engine/UI/Button.h"
#include "../Engine/ComponentManager.h"

namespace PM
{
	class GameObject;
}

class ActionManager : public PM::Component// GSComponent
{
public:
	ActionManager();
	virtual ~ActionManager();
	void Update(double) override;
	bool CheckAction(SkillType st);
	std::list<Act::Action*> GetAllAction() { return actions; }
	void SetAction(PM::LevelData* data);
	Act::Action* GetAction(SkillType st);

protected:
	std::list<Act::Action*> actions;
};
