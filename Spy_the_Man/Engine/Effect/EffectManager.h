/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include <list>
#include "../Engine/Component.h"
namespace math
{
	class TransformMatrix;
}

class Effect;

class EffectManager : public PM::Component
{

public:
	EffectManager() : Component("EffectManager") {}

	~EffectManager(){Clear();}

	void Update(double) override;

	void DrawAll(math::TransformMatrix& displayMatrix);


	void Add(Effect* e);
	
	void Clear();
private:
	std::list<Effect*> effects;
};

