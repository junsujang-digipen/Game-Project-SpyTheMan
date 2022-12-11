#pragma once
#include <vector>

#include "../../Engine/Component.h"
#include "../../Engine/Declaration.h"


namespace PM
{
	class LevelData;
}

namespace PM
{
	namespace UI
	{
		class Rect;
		class Text;
		class Button;
		class UIEntity;
	}
}


class Phone : public PM::Component // GS
{

public:
	Phone();
	~Phone();
	void Update(double) override;
	void AddSkill(PM::UI::Button* Button);
	void setIsValid(bool tf) override;
	void TurnOn(bool set);
	
private:
	void PositionUpdate(double dt);
	void SetSkill();

	std::vector<PM::UI::Button*> skillList;
	PM::UI::Button* settingButton;

 
	double timer{0};
	
	
	static constexpr float ImageAlpha{ 100 };
	
	
	static constexpr glm::vec3 color = glm::vec3{ 45,61,131 };
	static constexpr float Sideinterval = 15.f;
	static constexpr float ButtonsInterval = 30.f;
};

