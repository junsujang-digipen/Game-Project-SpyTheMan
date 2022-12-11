#pragma once
#include "../Engine/Component.h"
#include "../Engine/Engine.h"
#include "../Engine/World.h"
#include "../Engine/Skill/SkillManager.h"
namespace PM
{
	class GameObject;
}

namespace Act
{
	class Action : PM::Component
	{
	public:
		Action(SkillType skilltype, PM::SkillManager::SkillData* data, int amount, float cooltime);
		void Update(double dt) override { if (timer < cooltime) { timer += (float)dt; } }
		~Action();
		int GetAmount() const { return amount; }
		float GetTimer() const { return timer; }
		void SetTimer(float time = 0) { timer = time; };
		float GetCooltime() const { return cooltime; }
		int GetTotalAmount() const { return TotalAmount; }
		bool CanDoAction()
		{
			return (amount > 0 && timer >= cooltime);
		}
		PM::SkillManager::SkillData* GetSkillData() { return skillData; }
		SkillType GetSkillType() { return skillType; }
		virtual void DoAction(PM::GameObject* obj) = 0;
	protected:
		const SkillType skillType;
		PM::SkillManager::SkillData* skillData{nullptr};
		const float cooltime{0};
		float timer{0};
		int amount{0};
		const int TotalAmount{ 0 };
		PM::GameObject* player{ nullptr };
	};


	
	template<typename T>
	class TAction : public Action
	{
	public:
		TAction(SkillType skilltype, PM::SkillManager::SkillData* data, int amount, float cooltime) : Action(skilltype,data,amount,cooltime) {}
		~TAction(){}
	private:
		void DoAction(PM::GameObject* obj) override;
	};
}



template <typename T>
void Act::TAction<T>::DoAction(PM::GameObject* obj)
{
	if (amount > 0)
	{
		new T(PM::SequenceType::Player,
			player,
			obj);
		
		amount--;
		timer = 0;
	}
}