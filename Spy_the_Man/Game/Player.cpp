/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Player.cpp
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 10/7/2021
-----------------------------------------------------------------*/
#include "../Engine/Engine.h"
#include "../Engine/Poodle.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Rect.h"
#include "../Engine/IOManager/ObjectData.h"
#include "../Engine/SoundManager.h"
#include "../Engine/Wave.h"
#include "Player.h"
#include "Object/ActivateVision.h"
#include "../Engine/Sequence/Sequence_base.h"

Player::Player(glm::vec2 position) : GameObject("player", position),
moveLeftKey(PM::InputKey::Keyboard::A),
moveRightKey(PM::InputKey::Keyboard::D),
runKey(PM::InputKey::Keyboard::Shift),
dashKey(PM::InputKey::Keyboard::Space),
drawPlayer(false),
isDead(false),
gotopos(false),
hide(false),
vent(false),
invincible(false),
run(false),
runpressed(false),
drone(false),
silent(false),
dash(false),
dashing(false),
hidetime(0) {
	this->AddGOComponent(new PM::RectCollision(collisionSize, this));
	AddGOComponent(new PM::Sprite("assets/player.json", this->GetEntityType()));
	dead = new PM::Sprite("assets/dead.json", this->GetEntityType());
	SetSize(playerSize);
	currState = &stateIdle;
	currState->Enter(this);
	AddGOComponent(new ActivateVision(this));
}

Player::~Player() {
	delete(dead);
}

void Player::Update(double dt) {
	GameObject::Update(dt);
	if (IsFaceLeft() == true) {
		runningPos = { GetCenter().x - 700, GetCenter().y };
		walkingPos = { GetCenter().x - 300, GetCenter().y };
	}
	else {
		runningPos = { GetCenter().x + 700, GetCenter().y };
		walkingPos = { GetCenter().x + 300, GetCenter().y };
	}
	if (drone == false && Engine::GetSoundManager().SoundFinished("dronesound")) {
		Engine::GetSoundManager().setSoundListener(GetCenter());
	}
	if (invisible == true) {
		invtime -= dt;
		if (closet == true) {
			invisible = false;
			invtime = 3;
		}
	}
	if (invtime <= 0) {
		invisible = false;
		hide = false;
		invtime = 3;
	}
	if (dash == true) {
		dashcounter += dt;
		if (dashcounter >= 5 && dashKey.IsKeyReleased() == true) {
			Engine::GetSoundManager().PlaySound("Dash", GetPosition());
			dashing = true;
			dashcounter = 0;
		}
	}
	if (disguisenow != DisguiseList::Player) {
		distime -= dt;
	}
	if (distime <= 0) {
		SetDisguise(DisguiseList::Player);
		distime = 15;
	}
	if (currState != &stateIdle) {
		Engine::GetGSComponent<PM::World>()->SetIsDetectNPC(false);
	}
	if (isDead == true) {
		alp -= 20;
	}
}

void Player::Draw(math::TransformMatrix& displayMatrix) const {
	if (hide == true) {
		GetGOComponent<PM::Sprite>()->SetTrans(true);
	}
	else {
		GetGOComponent<PM::Sprite>()->SetTrans(false);
	}
	if (closet == true) {
		GetGOComponent<PM::Sprite>()->SetCloset(true);
	}
	else {
		GetGOComponent<PM::Sprite>()->SetCloset(false);
	}
	PM::push_settings();
	PM::use_shader("Player");
	GameObject::Draw(displayMatrix);
	PM::pop_settings();
	if (drawtrap == true) {
		PM::push_settings();
		PM::apply_matrix(PM::getCameraMat());
		PM::set_fill_color(128, 0, 128, 10);
		if (trapleft == true) {
			PM::draw_rect(GetCenter().x - 600, GetPosition().y, 500, 100);
		}
		else {
			PM::draw_rect(GetCenter().x + 100, GetPosition().y, 500, 100);
		}
		PM::pop_settings();
	}
	if (drawpush == true) {
		PM::push_settings();
		PM::apply_matrix(PM::getCameraMat());
		PM::set_fill_color(0, 242, 222, 30);
		PM::no_outline();
		PM::rect_center();
		if (pushleft == true) {
			PM::draw_rect(GetCenter().x - 200, GetCenter().y, 200, 30);
			PM::draw_triangle(GetCenter().x - 300, GetCenter().y - 50, GetCenter().x - 300, GetCenter().y + 50, GetCenter().x - 350, GetCenter().y);
		}
		else {
			PM::draw_rect(GetCenter().x + 200, GetCenter().y, 200, 30);
			PM::draw_triangle(GetCenter().x + 300, GetCenter().y - 50, GetCenter().x + 300, GetCenter().y + 50, GetCenter().x + 350, GetCenter().y);
		}
		PM::pop_settings();
	}
	if (distime <= 5) {
		PM::push_settings();
		PM::apply_matrix(PM::getCameraMat());
		PM::yes_fill();
		PM::no_outline();
		PM::set_fill_color(255, 0, 0);
		PM::draw_rect(GetCenter().x - 15, GetCenter().y + 100, 30, distime * 50 / 5);
		PM::no_fill();
		PM::yes_outline();
		PM::set_line_width(3);
		PM::draw_rect(GetCenter().x - 15, GetCenter().y + 100, 30, 50);
		PM::pop_settings();
	}
	if (isDead == true) {
		PM::push_settings();
		PM::apply_NDC();
		PM::set_fill_color(255, 0, 0, alp);
		PM::no_outline();
		PM::draw_rect(0, 0, Engine::GetWindow().GetSize().x, Engine::GetWindow().GetSize().y);
		PM::pop_settings();
	}
}

void Player::CollisionEnter(Entity* obj) {
	switch (obj->GetEntityType()) {
	case EntityType::Lazer: 
		if (dashing == false && vent == false) { SetDead(true); }
		break;
	default:break;
	}
}

bool Player::CanCollideWith(EntityType objectBType) {
	switch (objectBType) {
	case EntityType::Wall: [[fallthrough]];
	case EntityType::Lazer: [[fallthrough]];
	case EntityType::SideDoor:
		return true;
		break;
	}
	return false;
}

void Player::CollisionStay(PM::Entity* obj) {
	math::rect2 collideRect = obj->GetGOComponent<PM::RectCollision>()->GetWorldCoorRect();
	math::rect2 playerRect = GetGOComponent<PM::RectCollision>()->GetWorldCoorRect();
	int left = collisionSize.Left();
	if (vent == true) {
		left = ventSize.Left();
	}
	if (obj->GetEntityType() == EntityType::SideDoor || (obj->GetEntityType() == EntityType::Wall)) {
		if (playerRect.Right() > collideRect.Left() && playerRect.Left() < collideRect.Left()) {
			SetPosition({ collideRect.Left() - playerRect.Size().x - left, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
		else if (playerRect.Left() < collideRect.Right() && playerRect.Right() > collideRect.Right()) {
			SetPosition({ collideRect.Right() - left, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
	}
}

void Player::GoTo(glm::vec2 destination) {
	gotopos = true;
	endPos = destination;
}

void Player::SetDisguise(DisguiseList disguisejob) {
	Engine::GetSoundManager().PlaySound("disguise", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
	auto spritePtr = GetGOComponent<PM::Sprite>();
	switch (disguisejob) {
	case(DisguiseList::Player):
		spritePtr->SetType(EntityType::Player);
		break;
	case(DisguiseList::Worker):
		spritePtr->SetType(EntityType::Worker_NPC);
		break;
	case(DisguiseList::Guard):
		spritePtr->SetType(EntityType::Enemy_NPC);
		break;
	}
	disguisenow = disguisejob;
}

void Player::SetPos(glm::vec2 pos) {
	SetPosition(pos);
	ChangeState(&this->stateIdle);
	GetGOComponent<PM::Sprite>()->PlayAnimation(1);
}

void Player::State_Idle::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PM::Sprite>()->PlayAnimation(1);
	player->hidetime = 0;
	detectTimer = 0;
}

void Player::State_Idle::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	if (player->chameleon == true) {
		player->hidetime += dt;
	}
	if (player->hidetime >= 1) {
		player->hide = true;
	}
	detectTimer += dt;
	if(detectTime <= detectTimer)
	{
		Engine::GetGSComponent<PM::World>()->SetIsDetectNPC(true);
	}
}

void Player::State_Idle::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->moveLeftKey.IsKeyDown() == true || player->moveRightKey.IsKeyDown() == true) {
		if (player->drone == false) {
			player->ChangeState(&player->stateMoving);
		}
	}
	if (player->gotopos == true) {
		player->ChangeState(&player->stateMovingtoPos);
	}
	if (player->vent == true) {
		player->ChangeState(&player->stateVentIdle);
		player->RemoveGOComponent<PM::RectCollision>();
		player->AddGOComponent(new PM::RectCollision(player->ventSize, player));
	}
	else if (player->hide == true && player->invisible == false) {
		player->ChangeState(&player->stateHiding);
	}
	if (player->isDead == true) {
		player->ChangeState(&player->stateDead);
	}
	if (player->isTalk == true) {
		player->ChangeState(&player->stateTalk);
	}
	if (player->dashing == true) {
		player->ChangeState(&player->statedash);
	}
}

void Player::State_Moving::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->run == true) {
		player->GetGOComponent<PM::Sprite>()->PlayAnimation(5);
	}
	else {
		player->GetGOComponent<PM::Sprite>()->PlayAnimation(2);
	}
	player->runtime = 0;
}

void Player::State_Moving::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	if (player->GetVelocity() == glm::vec2{ 0, 0 }) {
		Engine::GetCamera().bindTarget(player);
	}
	else {
		if (player->run == true) {
			Engine::GetCamera().bindTargetPos(player->runningPos);
			player->runtime += dt;
			if (player->silent == false) {
				if (player->runtime >= 0.4) {
					if (player->runwave == false) {
						Engine::GetSoundManager().PlaySound("run2", player->GetPosition());
						new PM::Wave({ player->GetCenter().x, player->GetPosition().y }, 3, 1, player->GetLayer());
						player->runwave = true;
						player->runtime = 0;
					}
				}
				else {
					player->runwave = false;
				}
			}
		}
		else {
			Engine::GetCamera().bindTargetPos(player->walkingPos);
		}
	}
	if (player->moveLeftKey.IsKeyDown() == true) {
		if (player->run == true) {
			player->SetVelocity({ -2 * Player::xVel, 0 });
		}
		else {
			player->SetVelocity({ -Player::xVel, 0 });
		}
		player->FaceLeft(true);
	}
	if (player->moveRightKey.IsKeyDown() == true) {
		if (player->run == true) {
			player->SetVelocity({ 2 * Player::xVel, 0 });
		}
		else {
			player->SetVelocity({ Player::xVel, 0 });
		}
		player->FaceLeft(false);
	}
}

void Player::State_Moving::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->moveLeftKey.IsKeyDown() == false && player->moveRightKey.IsKeyDown() == false) {
		player->ChangeState(&player->stateIdle);
		player->SetVelocity({ 0, 0 });
		Engine::GetCamera().bindTarget(player);
	}
	if (player->gotopos == true) {
		player->ChangeState(&player->stateMovingtoPos);
		Engine::GetCamera().bindTarget(player);
	}
	if (player->isDead == true) {
		player->ChangeState(&player->stateDead);
		player->SetVelocity({ 0, 0 });
		Engine::GetCamera().bindTarget(player);
	}
	if (player->runKey.IsKeyDown() == true && player->runpressed == false) {
		player->run = true;
		player->runpressed = true;
		player->ChangeState(&player->stateMoving);
		Engine::GetCamera().bindTarget(player);
	}
	if (player->runKey.IsKeyDown() == false && player->runpressed == true) {
		player->run = false;
		player->runpressed = false;
		player->ChangeState(&player->stateMoving);
		Engine::GetCamera().bindTarget(player);
	}
	if (player->isTalk == true) {
		player->ChangeState(&player->stateTalk);
		player->SetVelocity({ 0, 0 });
		Engine::GetCamera().bindTarget(player);
	}
	if (player->drone == true) {
		player->ChangeState(&player->stateIdle);
		player->SetVelocity({ 0, 0 });
		Engine::GetCamera().bindTarget(player);
	}
	if (player->dashing == true) {
		player->ChangeState(&player->statedash);
	}
	if (player->hide == true && player->closet == true) {
		player->ChangeState(&player->stateHiding);
	}
}

void Player::State_MovingtoPos::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PM::Sprite>()->PlayAnimation(2);
}

void Player::State_MovingtoPos::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	if (player->endPos.x < player->GetPosition().x + player->collisionSize.Left()) {
		player->SetVelocity({ -Player::xVel, 0 });
		player->FaceLeft(true);
	}
	if (player->endPos.x > player->GetPosition().x + player->collisionSize.Left()) {
		player->SetVelocity({ Player::xVel, 0 });
		player->FaceLeft(false);
	}
	if (abs(player->endPos.x - (player->GetPosition().x + player->collisionSize.Left())) < Player::xVel * dt) {
		player->SetPosition(glm::vec2{ player->endPos.x - player->collisionSize.Left(), player->endPos.y });
		player->gotopos = false;
	}
}

void Player::State_MovingtoPos::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->gotopos == false) {
		player->ChangeState(&player->stateIdle);
		player->SetVelocity({ 0, 0 });
	}
	if (player->moveLeftKey.IsKeyDown() == true || player->moveRightKey.IsKeyDown() == true) {
		player->SetInterrupted(true);
		player->ChangeState(&player->stateMoving);
		player->gotopos = false;
	}
	if (player->isDead == true) {
		player->ChangeState(&player->stateDead);
		player->SetVelocity({ 0, 0 });
		player->SetInterrupted(true);
		player->gotopos = false;
	}
	if (player->isTalk == true) {

		player->ChangeState(&player->stateTalk);
		player->SetVelocity({ 0, 0 });
		player->SetInterrupted(true);
		player->gotopos = false;
	}
	if (player->drone == true) {
		player->ChangeState(&player->stateIdle);
		player->SetVelocity({ 0, 0 });
		player->SetInterrupted(true);
		player->gotopos = false;
	}
	if (player->dashing == true) {
		player->ChangeState(&player->statedash);
	}
}

void Player::State_Hiding::Enter(GameObject*) {
}

void Player::State_Hiding::Update(GameObject*, double) {
}

void Player::State_Hiding::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->moveLeftKey.IsKeyDown() == true || player->moveRightKey.IsKeyDown() == true) {
		if (player->drone == false) {
			player->ChangeState(&player->stateMoving);
			player->hide = false;
			player->closet = false;
		}
	}
	else if (player->hide == false) {
		player->ChangeState(&player->stateIdle);
	}
	if (player->isDead == true) {
		player->ChangeState(&player->stateDead);
		player->hide = false;
	}
	if (player->isTalk == true) {
		player->ChangeState(&player->stateTalk);
		player->hide = false;
	}
	if (player->invisible == true) {
		player->ChangeState(&player->stateIdle);
		player->closet = false;
	}
	if (player->dashing == true) {
		player->ChangeState(&player->statedash);
	}
}

void Player::State_VentIdle::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PM::Sprite>()->PlayAnimation(3);
}

void Player::State_VentIdle::Update(GameObject*, double) {
	//Player* player = static_cast<Player*>(object);
}

void Player::State_VentIdle::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->moveLeftKey.IsKeyDown() == true || player->moveRightKey.IsKeyDown() == true) {
		if (player->drone == false) {
			player->ChangeState(&player->stateVenting);
		}
	}
	if (player->gotopos == true) {
		player->ChangeState(&player->stateVenttoPos);
	}
	if (player->vent == false) {
		player->ChangeState(&player->stateIdle);
		player->RemoveGOComponent<PM::RectCollision>();
		player->AddGOComponent(new PM::RectCollision(player->collisionSize, player));
	}
	if (player->isDead == true) {
		player->ChangeState(&player->stateDead);
	}
	if (player->isTalk == true) {
		player->ChangeState(&player->stateTalk);
	}
}

void Player::State_Venting::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PM::Sprite>()->PlayAnimation(4);
}

void Player::State_Venting::Update(GameObject* object, double /*dt*/) {
	Player* player = static_cast<Player*>(object);
	if (player->moveLeftKey.IsKeyDown() == true) {
		player->SetVelocity({ -Player::xVel / 2, 0 });
		player->FaceLeft(true);
	}
	if (player->moveRightKey.IsKeyDown() == true) {
		player->SetVelocity({ Player::xVel / 2, 0 });
		player->FaceLeft(false);
	}
}

void Player::State_Venting::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->moveLeftKey.IsKeyDown() == false && player->moveRightKey.IsKeyDown() == false) {
		player->ChangeState(&player->stateVentIdle);
		player->SetVelocity({ 0, 0 });
	}
	if (player->gotopos == true) {
		player->ChangeState(&player->stateVenttoPos);
	}
	if (player->isDead == true) {
		player->ChangeState(&player->stateDead);
		player->SetVelocity({ 0, 0 });
	}
	if (player->isTalk == true) {
		player->ChangeState(&player->stateTalk);
		player->SetVelocity({ 0, 0 });
	}
	if (player->drone == true) {
		player->ChangeState(&player->stateVentIdle);
		player->SetVelocity({ 0, 0 });
	}
}

void Player::State_VenttoPos::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PM::Sprite>()->PlayAnimation(4);
}

void Player::State_VenttoPos::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	if (player->endPos.x < player->GetPosition().x + player->collisionSize.Left()) {
		player->SetVelocity({ -Player::xVel / 2, 0 });
		player->FaceLeft(true);
	}
	if (player->endPos.x > player->GetPosition().x + player->collisionSize.Left()) {
		player->SetVelocity({ Player::xVel / 2, 0 });
		player->FaceLeft(false);
	}
	if (abs(player->endPos.x - (player->GetPosition().x + player->collisionSize.Left())) < Player::xVel * dt / 2) {
		player->SetPosition(glm::vec2{ player->endPos.x - player->collisionSize.Left(), player->endPos.y });
		player->gotopos = false;
	}
}

void Player::State_VenttoPos::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->gotopos == false) {
		player->ChangeState(&player->stateVentIdle);
		player->SetVelocity({ 0, 0 });
	}
	if (player->isDead == true) {
		player->ChangeState(&player->stateDead);
		player->SetVelocity({ 0, 0 });
		player->gotopos = false;
	}
	if (player->isTalk == true) {
		player->ChangeState(&player->stateTalk);
		player->SetVelocity({ 0, 0 });
		player->gotopos = false;
	}
	if (player->drone == true) {
		player->ChangeState(&player->stateVentIdle);
		player->SetVelocity({ 0, 0 });
		player->gotopos = false;
	}
}

void Player::State_Killing::Enter(GameObject*) {}

void Player::State_Killing::Update(GameObject*, double) {}

void Player::State_Killing::TestForExit(GameObject*) {}

void Player::State_Dead::Enter(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	player->RemoveGOComponent<PM::Sprite>();
	player->AddGOComponent(new PM::Sprite("assets/dead.json", player->GetEntityType()));
	Engine::GetSoundManager().Pause();
	Engine::GetSoundManager().setStop(true);
	Engine::GetSoundManager().Reset();
	Engine::GetSoundManager().PlaySound("die", player->GetPosition());
	if (player->IsFaceLeft() == true) {
		player->SetPosition({ player->GetPosition().x - 250, player->GetPosition().y });
	}
	player->SetSize({ 500, 250 });
	player->GetGOComponent<PM::Sprite>()->PlayAnimation(1);
}

void Player::State_Dead::Update(GameObject*, double) {}

void Player::State_Dead::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->GetGOComponent<PM::Sprite>()->IsAnimationDone() == true) {
		player->ChangeState(&player->stateDestroy);
	}
	if (player->isDead == false) {
		player->ChangeState(&player->stateIdle);
	}
}

void Player::State_Talk::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PM::Sprite>()->PlayAnimation(1);
}

void Player::State_Talk::Update(GameObject*, double) {}

void Player::State_Talk::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->isTalk == false) {
		player->ChangeState(&player->stateIdle);
	}
}

void Player::State_Dash::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PM::Sprite>()->PlayAnimation(5);
	player->dashing = false;
	player->dashtime = 0.2;
	if (player->IsFaceLeft() == true) {
		player->SetVelocity({ -3000, 0 });
	}
	else {
		player->SetVelocity({ 3000, 0 });
	}
}

void Player::State_Dash::Update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->dashtime -= dt;
}

void Player::State_Dash::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->dashtime <= 0) {
		player->ChangeState(&player->stateIdle);
		player->SetVelocity({ 0, 0 });
	}
}

void Player::State_Destroy::Enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<PM::Sprite>()->PlayAnimation(2);
}

void Player::State_Destroy::Update(GameObject*, double) {}

void Player::State_Destroy::TestForExit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->destroy = true;
}