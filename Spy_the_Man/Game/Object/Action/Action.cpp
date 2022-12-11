#include "Action.h"

Act::Action::Action(SkillType skilltype, PM::SkillManager::SkillData* data, int amount, float cooltime) : skillType(skilltype), skillData(data), cooltime(cooltime), amount(amount), TotalAmount(amount)
{
	player = Engine::GetGSComponent<PM::World>()->GetPlayerPtr();
}
Act::Action::~Action()
{
	
}
