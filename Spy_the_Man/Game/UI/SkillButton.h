#pragma once
#include "../../Engine/UI/Button.h"
#include "../../Engine/Engine.h"
namespace PM {
	class GameObject;
}

namespace Act {
	class Action;
}

class SkillButton : public PM::UI::ImageButton
{
public:
	SkillButton(Act::Action* act, PM::GameObject* obj, std::string filepath, UILayer layer = UILayer::SKILLUI);
	virtual ~SkillButton() = default;

	void Update(double dt) override;
	void Draw(math::TransformMatrix& displayMatrix) const override;
	void DrawCooltime(math::TransformMatrix& displayMatrix) const;
	void DrawAmount(math::TransformMatrix& displayMatrix) const;
	static constexpr float UNDERBARSIZE{10};
private:
	Act::Action* act{ nullptr };
	PM::GameObject* obj{ nullptr };
	bool valid = true;
};

template<typename T>
class SequenceButton : public PM::UI::ImageButton
{
public:
	SequenceButton(std::string name, PM::GameObject* obj, std::string buttonImagepath, UILayer layer = UILayer::OBJECTUI);
	virtual ~SequenceButton() = default;
	void Update(double dt) override;
private:
	
	PM::GameObject* obj{ nullptr };
};


template <typename T>
SequenceButton<T>::SequenceButton(std::string name, PM::GameObject* obj, std::string filepath, UILayer layer)
	: ImageButton(name, filepath, { 0,0 }, { 0,0 }, layer, false, false), obj(obj)
{

}


template <typename T>
void SequenceButton<T>::Update(double)
{
	if (this->IsReleased() == true)
	{
		new T(PM::SequenceType::Player,
			Engine::GetGSComponent<PM::World>()->GetPlayerPtr(),
			obj);
	}
}
