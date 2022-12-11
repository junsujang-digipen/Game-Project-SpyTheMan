/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include <list>
#include "EffectManager.h"

#include <vector>

#include "Effect.h"

void EffectManager::Update(double dt)
{
	std::vector<Effect*> destroylist;

	for (Effect* effect : effects)
	{
		effect->Update(dt);
		if (effect->IsEnd())
		{
			destroylist.push_back(effect);
		}
	}
	for (Effect* ef : destroylist)
	{
		effects.remove(ef);
		delete ef;
	}
	destroylist.clear();
}

void EffectManager::DrawAll(math::TransformMatrix& displayMatrix)
{
	for (Effect* e : effects)
	{
		e->Draw(displayMatrix);
	}
}

void EffectManager::Add(Effect* e)
{
	effects.push_back(e);
}

void EffectManager::Clear()
{
	for (auto e : effects)
	{
		delete e;
	}
	effects.clear();
}
