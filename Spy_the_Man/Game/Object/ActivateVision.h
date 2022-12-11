#pragma once
#include "../../Engine/Component.h"

namespace PM {
	class GameObject;
}

class ActivateVision : public PM::Component
{
public:
	ActivateVision(PM::GameObject* thisObject);
	void Update(double) override;
private:
	PM::GameObject* object;
};