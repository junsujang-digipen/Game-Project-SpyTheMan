#include "ActionManager.h"

#include "Action.h"
#include "../../Sequence/ChameleonSequence.h"
#include "../../Sequence/DroneSequence.h"
#include "../../Sequence/InvisibleSequence.h"
#include "../../Sequence/VisualSequence.h"
#include "../../Sequence/SilentRunSequence.h"
#include "../../Sequence/TrapSequence.h"
#include "../../Sequence/PushSequence.h"
#include "../../Sequence/DashSequence.h"
#include "../../Sequence/FlashbombSequence.h"
#include "../../Sequence/FindSequence.h"
#include "../Engine/Engine.h"

ActionManager::ActionManager() : Component("ActionManager")
{
	//SetAction();
}

ActionManager::~ActionManager()
{
	for (auto action : actions)
	{
		delete action;
	}
	actions.clear();
}


void ActionManager::Update(double dt)
{
	for(auto action : actions)
	{
		action->Update(dt);
	}
}

bool ActionManager::CheckAction(SkillType st)
{
	for(auto& act : actions)
	{
		if(act->GetSkillType() == st)
		{
			return true;
		}
	}
	return false;
}

Act::Action* ActionManager::GetAction(SkillType st)
{
	for (auto& act : actions)
	{
		if (act->GetSkillType() == st)
		{
			return act;
		}
	}
	return nullptr;
}

void ActionManager::SetAction(PM::LevelData* data)
{
	std::vector<SkillType> skillType;

	for(int i = 0; i < static_cast<int>(SkillType::Null);++i )
	{
		if(data->GetSkillset(static_cast<SkillType>(i)))
		{
			skillType.push_back(static_cast<SkillType>(i));
		}
	}
	for (SkillType& skill : skillType)
	{
		PM::SkillManager::SkillData* sData  = Engine::GetSkillManager().GetSkillData(skill);
		int amount = sData->use;
		float cool = sData->cooldown;
		switch (skill) {
		case SkillType::Drone:actions.push_back(new Act::TAction<DroneSequence>(SkillType::Drone, sData,amount,cool)); break;
		case SkillType::DroneSound: 
			for (auto a : actions) {
				if (a->GetSkillType() == SkillType::Drone) {
					Engine::GetSkillManager().CurrSkills().push_back(SkillType::DroneSound);
				}
			}
			break;
		case SkillType::Chameleon: {
			Act::Action* action = new Act::TAction<ChameleonSequence>(SkillType::Chameleon, sData, amount, cool);
			action->DoAction(Engine::GetGSComponent<PM::World>()->GetPlayerPtr());
			break; 
		}
		case SkillType::Invisible:actions.push_back(new Act::TAction<InvisibleSequence>(SkillType::Invisible, sData, amount, cool)); break;
		case SkillType::Visualize:actions.push_back(new Act::TAction<VisualSequence>(SkillType::Visualize, sData, amount, cool)); break;
		case SkillType::SilentRun: {
			Act::Action* action = new Act::TAction<SilentRunSequence>(SkillType::SilentRun, sData, amount, cool);
			action->DoAction(Engine::GetGSComponent<PM::World>()->GetPlayerPtr());
			break;
		}
		case SkillType::Assassinate:
			break;
		case SkillType::SlowTrap:actions.push_back(new Act::TAction<TrapSequence>(SkillType::SlowTrap, sData, amount, cool)); break;
		case SkillType::Detect:actions.push_back(new Act::TAction<FindSequence>(SkillType::Detect, sData, amount, cool)); break;
		case SkillType::Push:actions.push_back(new Act::TAction<PushSequence>(SkillType::Push, sData, amount, cool)); break;
		case SkillType::Dash: {
			Act::Action* action = new Act::TAction<DashSequence>(SkillType::Dash, sData, amount, cool);
			action->DoAction(Engine::GetGSComponent<PM::World>()->GetPlayerPtr());
			break;
		}
		case SkillType::Flashbomb:actions.push_back(new Act::TAction<FlashbombSequence>(SkillType::Flashbomb, sData, amount, cool)); break;
		case SkillType::Null: break;
		default:;
		}
	}
}
