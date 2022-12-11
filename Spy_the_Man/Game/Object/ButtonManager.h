#pragma once
#include <vector>

#include "../../Engine/Component.h"


namespace PM {
	namespace UI {
		class Button;
	}

	class GameObject;
}

class ButtonManager : public PM::Component
{
public:
	ButtonManager(PM::GameObject* obj);
	virtual ~ButtonManager();

	void AddButton(PM::UI::Button* button);

	PM::UI::Button* GetButton(std::string name);
	void RemoveButton(std::string name);
	void Update(double) override;
	void Activate(bool targeted);
	
protected:
	PM::GameObject* objPtr;
	bool activate{ false };
	bool check{ false };
	std::vector<PM::UI::Button*> buttons_;
};
