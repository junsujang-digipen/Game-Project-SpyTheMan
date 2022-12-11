#include "SkillButton.h"

#include "../SkillString.h"
#include "../Object/Action/Action.h"
#include "../Engine/GameObject.h"
#include "../Player.h"

SkillButton::SkillButton(Act::Action* act, PM::GameObject* obj, std::string filepath, UILayer layer)
	: ImageButton(act->GetSkillData()->name, filepath, { 0,0 }, { 0,0 }, layer, false, true), act(act), obj(obj)
{
	SetHelpText(PM::SkillString(act->GetSkillType()).SS1);
}
void SkillButton::Draw(math::TransformMatrix& displayMatrix) const
{
	glm::vec2 pos = GetPosition();
	glm::vec2 Bsize = GetSize();
	glm::vec4 c = GetStateColor();
	PM::push_settings();
	PM::apply_matrix(displayMatrix.matrix);
	PM::no_outline();
	PM::set_fill_color(c.x, c.y, c.z, c.a);
	PM::draw_rect(pos.x, pos.y, Bsize.x, Bsize.y);
	texturePtr->Draw(displayMatrix, GetPosition() + glm::vec2{ 5,5 }, GetSize() - glm::vec2{ 10,10 });
	if (valid == false) {
		PM::set_fill_color(0, 0, 0, 200);
		PM::draw_rect(pos.x, pos.y, Bsize.x, Bsize.y);
	}
	PM::pop_settings();
	DrawCooltime(displayMatrix);
	DrawAmount(displayMatrix);
}

void SkillButton::DrawAmount(math::TransformMatrix& displayMatrix) const
{
	glm::fvec2 pos = GetPosition();
	glm::fvec2 Bsize = GetSize();
	int amount = act->GetAmount();
	int totalAmount = act->GetTotalAmount();
	PM::push_settings();
	PM::apply_matrix(displayMatrix.matrix);
	float colSIze = Bsize.x / static_cast<float>(totalAmount);
	PM::yes_outline();
	PM::set_fill_color(0, 0, 0, 0);
	PM::draw_rect(pos.x, pos.y - UNDERBARSIZE, Bsize.x, UNDERBARSIZE);
	for (int i = 0; i < totalAmount; i++)
	{
		glm::vec4 color_ = (amount > 0) ? glm::vec4{ 0,255,0,255 } : glm::vec4{ 0,0,0,0 };
		PM::set_fill_color(color_.r, color_.g, color_.b, color_.w);
		PM::draw_rect(pos.x + colSIze*i, pos.y - UNDERBARSIZE, colSIze, UNDERBARSIZE);
		amount--;
	}
	PM::pop_settings();
}

void SkillButton::DrawCooltime(math::TransformMatrix& displayMatrix) const
{
	glm::vec2 pos = GetPosition();
	glm::vec2 Bsize = GetSize();
	float timePer = act->GetTimer()/act->GetCooltime();
	PM::push_settings();
	PM::apply_matrix(displayMatrix.matrix);
	PM::no_outline();
	PM::set_fill_color(0, 0, 0, 50);
	PM::draw_rect(pos.x, pos.y, Bsize.x, Bsize.y - Bsize.y * timePer);
	PM::pop_settings();
}



void SkillButton::Update(double)
{
	PM::World* world = Engine::GetGSComponent<PM::World>();
	Player* playerptr = dynamic_cast<Player*>(world->GetPlayerPtr());
	if (playerptr->IsVent() == true) {
		valid = false;
	}
	else {
		valid = true;
	}
	if(this->IsReleased() == true && valid == true)
	{
		if(act->CanDoAction() == true)
		{
			act->DoAction(obj);
		}
	}
}

