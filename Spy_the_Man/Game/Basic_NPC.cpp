/*--------------------------------------------------------------
File Name: Basic_NPC.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 4/3/2021
-----------------------------------------------------------------*/
#include "Basic_NPC.h"
#include "../Engine/Sprite.h"
#include "../Engine/Collision.h"
#include "../Engine/Engine.h"
#include "Master_AI.h"
#include "../Game/Sequence/NPC_OpenDoorSequence.h"
#include "Player.h"
#include "../Engine/Helper_func_map.h"
#include "Object/Stair.h"
#include "NPC_Worker_AI.h"
#include "Enemy_soldier_AI.h"
#include "Enemy_soldier_senior_AI.h"
#include "Task_AI_basePosition.h"
#include "Object/ButtonManager.h"

#include "Object/Push.h"
#include "Sequence/KillAISequence.h"
#include "UI/SkillButton.h"

bool Basic_NPC::chasing(PM::GameObject* target, double sp, double /*dt*/)
{
	if (target->DoesCollideWith(this) == true) {
		return true;
	}
	go(target->GetPosition(), sp);
	return false;
}

void Basic_NPC::BasicAnimation()
{
	if (sight.x < 0) {
		FaceLeft(true);
	}
	else if (sight.x > 0) {
		FaceLeft(false);
	}
	
	PM::Sprite* sprite = GetGOComponent<PM::Sprite>();
	if (sprite->GetCurrentAnim() != 3) {
		
		if (GetVelocity().x == 0) {
			if (sprite->GetCurrentAnim() != 1) {
				GetGOComponent<PM::Sprite>()->PlayAnimation(1);
			}
		}
		else {
			if (std::abs(GetVelocity().x) > 350. && sprite->GetCurrentAnim() != 5) {
				sprite->PlayAnimation(5);
			}
			else if (std::abs(GetVelocity().x) < 350. && sprite->GetCurrentAnim() != 2) {
				GetGOComponent<PM::Sprite>()->PlayAnimation(2);
			}

		}
	}
	//GetGOComponent<PM::Sprite>()->PlayAnimation(0);
}
void Basic_NPC::loadBasicMarkTexture()
{
	headMarkImage.push_back(Engine::GetTextureManager().Load("assets/npcicon/ques.png"));
	headMarkImage.push_back(Engine::GetTextureManager().Load("assets/npcicon/excl.png"));
	headMarkImage.push_back(Engine::GetTextureManager().Load("assets/npcicon/slow.png"));
	headMarkImage.push_back(Engine::GetTextureManager().Load("assets/npcicon/stun.png"));
	headMarkImage.push_back(Engine::GetTextureManager().Load("assets/npcicon/wind.png"));

}

Basic_NPC::Basic_NPC(PM::GameObject* p, std::string name, glm::vec2 position, EntityType t,NPCLevelList l, bool isControlled_, std::string spritePath) :
	NPC_base(name, position), isControlled(isControlled_),type(t),NPClevel(l)
{
	
	player = p;
	AddGOComponent(new PM::Sprite(spritePath, t));
	glm::vec2 npcSize{ 250, 250 };
	math::irect2 collisionSize{ {70, 0}, {180, 200} };
	ChangeState(&state_nothing);
	AddGOComponent(new PM::RectCollision(collisionSize, this));
	if (Engine::GetGSComponent<Master_AI>() != nullptr) {
		Engine::GetGSComponent<Master_AI>()->EnrollNPC(this,isControlled);
	}
	//AddAction(new Act::Talk());
	loadBasicMarkTexture();
	SetSize(npcSize);
	GetGOComponent<ButtonManager>()->AddButton(new SequenceButton<Kill_AI>(
		"Kill_AI", this, "assets/GameObject/UI/Assassination.png"));
}

Basic_NPC::~Basic_NPC()
{
	if (isControlled == false) {
		for (auto& T : TaskAIComponents) {
			delete T;
		}
	}
	TaskAIComponents.clear();

	for (auto& obj : dropItems) {
		delete (obj);
	}
	dropItems.clear();
}

void Basic_NPC::Update(double dt)
{
	PM::World* world = Engine::GetGSComponent<PM::World>();
	Player* playerptr = dynamic_cast<Player*>(world->GetPlayerPtr());
	if (playerptr->IsDead() == true) {
		dt = 0;
	}
	if (playerptr->IsVent() == true) {
		GetGOComponent<ButtonManager>()->GetButton("Kill_AI")->SetLock(true);
	}
	else {
		GetGOComponent<ButtonManager>()->GetButton("Kill_AI")->SetLock(false);
	}
	BasicAnimation();
	PM::NPC_base::Update(dt);
	if (ImportantTask != nullptr && ImportantTask->getIsValid() ==false) {
		delete ImportantTask;
		ImportantTask = nullptr;

	}
	if (stunTime < 0) {
		stunTime = 0;
		setSpeed();
		MarkState = 0;
	}
	else if(stunTime > 0) {
		setSpeed(0);
		stunTime -= dt;
	}

	if (SoundDetact < 0.) {
		SoundDetact = 0.;
		MarkState = 0;
	}
	else if (SoundDetact > 0.) {
		SoundDetact -= dt;
	}
	if (WindMarkTime > 0) {
		WindMarkTime -= dt;
	}
	else if(WindMarkTime<0){
		WindMarkTime = 0;
		MarkState = 0;
	}
}

void Basic_NPC::Draw(math::TransformMatrix& displayMatrix) const
{
	NPC_base::Draw(displayMatrix);
	constexpr float qMarkSize{ 29.f };
	constexpr float eMarkSize{ 12.5f };
	constexpr float slowMarkSize{ 70.f };
	constexpr float stunMarkSize{ 73.75f };
	constexpr float windMarkSize{ 41.f };
	constexpr float MarkHei{ 50 };
	glm::vec2 NPCSize{ GetSize() };
	switch (MarkState)
	{
	case 1:headMarkImage[0]->Draw(displayMatrix, GetPosition() + glm::vec2{ NPCSize.x / 2 - qMarkSize / 2,NPCSize.y }, { qMarkSize,MarkHei });break;
	case 2:headMarkImage[1]->Draw(displayMatrix, GetPosition() + glm::vec2{ NPCSize.x / 2 - eMarkSize / 2,NPCSize.y }, { eMarkSize,MarkHei });break;
	case 3:headMarkImage[2]->Draw(displayMatrix, GetPosition() + glm::vec2{ NPCSize.x / 2 - slowMarkSize / 2,NPCSize.y }, { slowMarkSize,MarkHei });break;
	case 4:headMarkImage[3]->Draw(displayMatrix, GetPosition() + glm::vec2{ NPCSize.x / 2 - stunMarkSize / 2,NPCSize.y }, { stunMarkSize,MarkHei });break;
	case 5:headMarkImage[4]->Draw(displayMatrix, GetPosition() + glm::vec2{ NPCSize.x / 2 - windMarkSize / 2,NPCSize.y }, { windMarkSize,MarkHei });break;
	default:
		break;
	}
	if (suspicionGaze != 0) {
		static const glm::vec3 colorValues[4]{ 
			glm::vec3{0.,255.,0.},
			glm::vec3{255.,255.,0.},
			glm::vec3{255.,150.,0.},
			glm::vec3{255.,0.,0.} };
		static int nowColNum{0};
		const double alphaVal{ helper::map(suspicionGaze,0.,10.,0.,255.) };
		constexpr double barY{ 20. };
		PM::push_settings();
		PM::apply_matrix(displayMatrix.matrix);
		nowColNum = static_cast<int>((suspicionGaze-1.)/33.33);
		double lerpVal{ helper::map((suspicionGaze - 1) / 33.33,(double)nowColNum,(double)nowColNum+1.,0.,1.)};
		glm::vec3 barColor{ helper::Lerp(colorValues[nowColNum], colorValues[nowColNum + 1],(float)lerpVal) };
		PM::set_fill_color(barColor.x, barColor.y, barColor.z, alphaVal);
		PM::set_line_color(255., 0., 0., alphaVal);
		PM::draw_rect(GetPosition().x, GetPosition().y + NPCSize.y - barY - 10, helper::map(suspicionGaze, 0., 100., 0., NPCSize.x), barY);

		PM::set_fill_color(0, 0, 0, alphaVal);
		PM::set_line_color(0, 0, 0, alphaVal);
		PM::no_fill();
		PM::set_line_width(3);
		PM::draw_rect(GetPosition().x, GetPosition().y + NPCSize.y - barY - 10, NPCSize.x, barY);
		PM::pop_settings();
	}


}

bool Basic_NPC::CanCollideWith(EntityType objectBType)
{
	switch (objectBType) {
	case EntityType::Wall: [[fallthrough]];
	case EntityType::Wave: [[fallthrough]];
	case EntityType::SideDoor: [[fallthrough]];
	case EntityType::Trap: [[fallthrough]];
	case EntityType::Flashbomb: [[fallthrough]];
	case EntityType::Push:
		return true;
	default: return false;
		break;
	}
}

void Basic_NPC::CollisionEnter(PM::Entity* obj)
{
	
	switch (obj->GetEntityType()) {
	case EntityType::Wall: collidingObj.push_back(obj);
		break;
	case EntityType::SideDoor:
		//opendoor and close
		new NPC_OpenDoorSequence{ PM::SequenceType::Other,this, dynamic_cast<GameObject*>(obj) };
		break;
	case EntityType::Wave:
		if (ImportantTask == nullptr) {
			AddImportantTaskAI(new Task_AI_basePosition{ this,obj->GetPosition(),{},false });
			SoundDetact = SoundDetactMax;
			setMarkState(1);
		}
		break;
	case EntityType::Trap: // some effect on head
		setMarkState(3);
		break;
	case EntityType::Flashbomb: // some effect on head
		setSpeed(0);
		stunTime = stunTimeMax;
		setMarkState(4);
		break;
	case EntityType::Push:
		setMarkState(5);
		break;
	default: break;

	}
}

void Basic_NPC::CollisionStay(PM::Entity* obj)
{
	PM::RectCollision* objCol{obj->GetGOComponent<PM::RectCollision>()};
	math::rect2 collideRect{};
	if (objCol != nullptr) {
		collideRect = objCol->GetWorldCoorRect();

	}
	math::rect2 enemyRect = GetGOComponent<PM::RectCollision>()->GetWorldCoorRect();

	switch (obj->GetEntityType()) {
	case EntityType::Wall: 
		if (enemyRect.Right() > collideRect.Left() && enemyRect.Left() < collideRect.Left()) {
			SetPosition({ collideRect.Left() - GetSize().x / 2.f - enemyRect.Size().x / 2.f, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
		else if (enemyRect.Left() < collideRect.Right() && enemyRect.Right() > collideRect.Right()) {
			SetPosition({ collideRect.Right() - GetSize().x / 2.f + enemyRect.Size().x / 2.f, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
		break;
	case EntityType::SideDoor:
		if (enemyRect.Right() > collideRect.Left() && enemyRect.Left() < collideRect.Left()) {
			SetPosition({ collideRect.Left() - GetSize().x/2.f - enemyRect.Size().x/2.f, GetPosition().y });
			//SetVelocity({ 0, GetVelocity().y });
		}
		else if (enemyRect.Left() < collideRect.Right() && enemyRect.Right() > collideRect.Right()) {
			SetPosition({ collideRect.Right() - GetSize().x / 2.f + enemyRect.Size().x / 2.f, GetPosition().y });
			//SetVelocity({ 0, GetVelocity().y });
		}
		break;
	case EntityType::Trap: // some effect on head
		setSpeed(50.);
		setMarkState(3);
		break;
	case EntityType::Push:
		if (collidingObj.size() == 0) {

			if (dynamic_cast<Push*>(obj)->IsLeft() == true) {
				SetVelocity({ -getRunSpeed(),0 });
			}
			else {
				SetVelocity({ getRunSpeed(),0 });
			}
			setMarkState(5);
			WindMarkTime = 0.1;
		}
		break;

	default: break;

	}
}

void Basic_NPC::CollisionExit(PM::Entity* obj)
{
	switch (obj->GetEntityType()) {
	case EntityType::Wall: collidingObj.remove(obj);
		break;
	case EntityType::Trap: [[fallthrough]];
	case EntityType::Flashbomb: // effect off
		MarkState = 0;
		setSpeed();
		break;
	case EntityType::Push:
		break;
	default: break;

	}
}

const double Basic_NPC::GazingSpeed(double dt)//TODO
{
	Player* playerPtr = dynamic_cast<Player*>(player);

	double dist{ glm::length(playerPtr->GetPosition() - this->GetPosition()) };
	double gSpeed{ helper::map(dist,0.,1000.,400.,6.) };
	static double gSpeedMultVal[2][3][3]{
		{//npc worker
			//player default, player worker, player enemy
			{{1.},{0.},{0.}}, // npc normal
			{{1.},{0.5},{0.3}}, // npc senior
			{{1.},{1.},{1.}} // npc master
		},
		{//npc enemy
			//player default, player worker, player enemy
			{{1.5},{0.35},{0.}}, // npc normal
			{{1.5},{1.},{0.5}}, // npc senior
			{{1.5},{1.5},{1.}} // npc master
		}
	};
	int NPCType{};
	int NPCLevelType{};
	int PlayerClothType{};
	switch (type)
	{
	case EntityType::Worker_NPC:
		NPCType = 0;
		break;
	case EntityType::Enemy_NPC:
		NPCType = 1;
		break;
	default:
		break;
	}
	switch (NPClevel)
	{
	case NPCLevelList::Normal:
		NPCLevelType = 0;
		break;
	case NPCLevelList::Senior:
		NPCLevelType = 1;
		break;
	case NPCLevelList::Master:
		NPCLevelType = 2;
		break;
	default:
		break;
	}

	switch (playerPtr->GetDisguise())
	{

	case DisguiseList::Player:
		PlayerClothType = 0;
		break;
	case  DisguiseList::Worker:
		PlayerClothType = 1;
		break;
	case DisguiseList::Guard:
		PlayerClothType = 2;
		break;
	default:
		break;
	}

	return gSpeed
		* gSpeedMultVal[NPCType][NPCLevelType][PlayerClothType] 
		* indiIncreaseGuardLevelSpeed *dt;
}

void Basic_NPC::setSight(glm::vec3 s)
{
	sight = s;
}
void Basic_NPC::setAI(PM::Component* ai)
{
	AddGOComponent(ai);

}
void Basic_NPC::go(glm::vec2 dest, double goSpeed) {
	PM::World* world = Engine::GetGSComponent<PM::World>();
	routesTempo.clear();

	std::vector<std::vector<int>> worldChunkTemp{};
	std::vector<PM::Chunk> worldChunks{ world->GetWorldChunk()[1] };;
	int chunkRowLeng{ static_cast<int>(world->GetChunkSize().x) };
	for (int i = 0;i < worldChunks.size();++i) {
		//int row{i/ (chunkRowLeng)};
		//int col{i% chunkRowLeng };
		worldChunkTemp.emplace_back();
		if ((i + 1) % chunkRowLeng != 0) {
			worldChunkTemp[i].push_back(i + 1);
		}
		if (i - 1 >= 0 && (i - 1) % chunkRowLeng != chunkRowLeng-1) {
			worldChunkTemp[i].push_back(i - 1);
		}
		if (i > 0) {
			for (auto& o : worldChunks[i - 1].GetObjects()) {
				if (o->GetEntityType() == EntityType::Wall) {
					glm::vec2 chunkCenterPos = worldChunks[i-1].GetPosition() + worldChunks[i-1].Getsize() / 2.f;
					if (o->GetPosition().x > chunkCenterPos.x && o->GetPosition().y< chunkCenterPos.y) {
						for (std::vector<int>::iterator itr = worldChunkTemp[i].begin();itr != worldChunkTemp[i].end();++itr) {
							if (*itr == i - 1) {
								worldChunkTemp[i].erase(itr);
								break;
							}
						}
					}
				}
			}
		}
		for (auto&o: worldChunks[i].GetObjects()) {
			if (o->GetEntityType() == EntityType::Wall) {
				glm::vec2 chunkCenterPos = worldChunks[i].GetPosition() + worldChunks[i].Getsize() / 2.f;
				if (o->GetPosition().x < chunkCenterPos.x && o->GetPosition().y < chunkCenterPos.y) {
					for (std::vector<int>::iterator itr = worldChunkTemp[i].begin();itr != worldChunkTemp[i].end();++itr) {
						if (*itr == i -1) {
							worldChunkTemp[i].erase(itr);
							break;
						}
					}
				}
				else if(o->GetPosition().x > chunkCenterPos.x && o->GetPosition().y < chunkCenterPos.y){
					for (std::vector<int>::iterator itr = worldChunkTemp[i].begin();itr != worldChunkTemp[i].end();++itr) {
						if (*itr == i + 1) {
							worldChunkTemp[i].erase(itr);
							break;
						}
					}
				}
			}
			else if (o->GetEntityType() == EntityType::Stair) {
				worldChunkTemp[i].push_back(
					world->GetChunkNumber(
						dynamic_cast<Stair*>(o)->GetTarget()));
			}
		}
	}
	std::vector<int> route{};
	glm::vec2 direc{};
	Stair* stairTemp{nullptr};
	int nowChunkNum{ world->GetChunkNumber(this) };
	FindRoute(worldChunkTemp, nowChunkNum,
		world->GetChunkNumber(dest), route);
	if (routesTempo.empty() == false) {
		route = routesTempo.begin()->second;

	}
	if (route.empty() == false) {
		direc = world->GetChunk(1, route.front()).GetPosition() + glm::vec2{ world->GetChunk(1, route.front()).Getsize().x / 2.f,0.f } - this->GetPosition();

		if (std::abs(route.front() - nowChunkNum) >= chunkRowLeng) {
			for (auto& obj : world->GetChunk(1, nowChunkNum).GetObjects()) {
				if (obj->GetEntityType() == EntityType::Stair) {
					if (world->GetChunkNumber(dynamic_cast<Stair*>(obj)->GetTarget()) == route.front()) {
						stairTemp = dynamic_cast<Stair*>(obj);
						direc = stairTemp->GetPosition() - this->GetPosition();
						if (/*abs(direc.x) <= GetSize().x*/obj->DoesCollideWith(this) == true) {
							SetPosition(stairTemp->GetTarget()->GetPosition() - glm::vec2{ GetSize().x / 2.f - stairTemp->GetSize().x / 2.f ,0.f });
						}
						break;
					}
				}
			}
		}
	}
	else {
		direc = dest -this->GetPosition();
	}
	
	setSight(glm::vec3{ glm::vec2{ direc.x },0 });
	glm::vec2 normV = glm::normalize(glm::vec2{ direc.x ,0.f });
	glm::vec2 velo{ normV * gsl::narrow_cast<float>(goSpeed) };

	const int baseRoomNum{ world->GetFloorNumber(dest) };
	const int ownerRoomNum{ world->GetFloorNumber(this->GetPosition()) };
	if (baseRoomNum == ownerRoomNum && direc.x*direc.x < GetSize().x* GetSize().x) {

	}
	else {
		SetVelocity(velo);
	}
	
}

bool Basic_NPC::FindRoute(std::vector<std::vector<int>> world, int nowPos,int destination, std::vector<int>& route)
{
	
	if (nowPos == destination) {
		routesTempo[static_cast<int>(route.size())] = route;
		return true;
	}
	PM::World* GameWorld = Engine::GetGSComponent<PM::World>();
	while (world[nowPos].size() != 0)
	{
		for (auto&index: world[nowPos]) {
			std::vector<int>::iterator itr{std::find(world[index].begin(), world[index].end(),nowPos)};
			if (itr != world[index].end()) {
				world[index].erase(itr);
			}
		}
		int newPos{};
		if (std::abs(nowPos - destination) < GameWorld->GetChunkSize().x) {
			newPos={ world[nowPos].front() };
			world[nowPos].erase(world[nowPos].begin());
		}
		else {
			newPos={ world[nowPos].back() };
			world[nowPos].erase(world[nowPos].end()-1);
		}
		
		route.push_back(newPos);
		FindRoute(world, newPos, destination, route);
		route.pop_back();
	}
	return false;
}

bool Basic_NPC::sameRoomCheck(PM::GameObject* obj, PM::GameObject* B) {
	if (B == nullptr) {
		B = this;
	}
	PM::World* world = Engine::GetGSComponent<PM::World>();
	if (world->GetRoom(B) == world->GetRoom(obj)) {
		return true;
	}
	return false;
}
const std::vector<int> Basic_NPC::getAroundChunk() {
	PM::World* world = Engine::GetGSComponent<PM::World>();
	return  world->GetAroundChunkNumber(GetPosition());
}

void Basic_NPC::setMarkState(int state)
{
	switch (state)
	{
	case 1:
		[[fallthrough]];
	case 2:
		if (MarkState != 3 && MarkState != 4 && MarkState != 5) {
			MarkState = state;
		}
		break;
	case 3:
		if (MarkState != 4 && MarkState != 5) {
			MarkState = state;
		}
		break;
	case 4:
		MarkState = state;
		break;
	case 5:
		if (MarkState != 4) {
			MarkState = state;
		}
		break;

	default:
		MarkState = state;
		break;
	}
}

void Basic_NPC::AddTaskAI(PM::Component* Task) {
	TaskAIComponents.push_back(Task);
}

void Basic_NPC::AddImportantTaskAI(PM::Component* Task)
{
	ImportantTask = Task;
	hasImportantTask = true;
}

PM::Component* Basic_NPC::getSelectedTask()
{
	if (hasImportantTask == true) {
		hasImportantTask = false;
		return ImportantTask;
	}
	++NumberofTask;
	if (TaskAIComponents.size()<=NumberofTask) {
		NumberofTask = 0;
	}
	unsigned Num{ NumberofTask };
	for (auto&t:TaskAIComponents) {
		if (Num == 0 && t->getIsValid() ==true) {
			return t;
		}
		else if(isControlled == false)
		{
			t->setIsValid(true);
		}
		--Num;
	}
	
	if (isControlled == true) {
		std::list<PM::Component*> deleteList{};
		for (auto&t:TaskAIComponents) {
			if (t->getIsValid() == false) {
				deleteList.push_back(t);
			}
		}
		for (auto& dl:deleteList) {
			TaskAIComponents.remove(dl);
			delete dl;
			dl = nullptr;
		}
	}
	return nullptr;
}

void Basic_NPC::removeTask(PM::Component* task)
{
	PM::Component* temp{task};
	TaskAIComponents.remove(task);
	delete temp;
}

bool Basic_NPC::isNeedNewTask()
{
	return TaskAIComponents.size() == 0;
}

void Basic_NPC::AddDropItem(PM::GameObject* item)
{
	dropItems.push_back(item);
}

void Basic_NPC::DropItems()
{
	PM::World* world = Engine::GetGSComponent<PM::World>();

	for (auto& obj : dropItems) {
		obj->SetPosition(GetPosition());
		world->Add(obj);
	}
	dropItems.clear();

}
void Basic_NPC::removeMe()
{
	if (GetGOComponent<PM::Collision>() != nullptr) {
		RemoveGOComponent<PM::Collision>();
	}
	DropItems();
	Destroy();
}
bool Basic_NPC::canSeePlayer() {
	Player* p = static_cast<Player*>(player);
	return stunTime <= 0 
		&& p->IsHide() == false 
		&& p->IsVent() == false 
		&& p->GetLayer() == this->GetLayer();
}
bool Basic_NPC::seeing() {
	glm::vec2 directionToPlayer{ glm::vec2{ player->GetPosition() - this->GetPosition() } };
	PM::World* world = Engine::GetGSComponent<PM::World>();
	if (dynamic_cast<Player*>(player)->IsTalk() == true) {
		return 0;
	}
	//const int EnemyChunkNum{ PM::GetChunkNumber(GetPosition()) };
	//const std::vector<int> EnemyAroundChunkNum{PM::GetAroundChunkNumber(EnemyChunkNum)};

	const std::vector<int> EnemyAroundChunkNum{ world->GetAroundChunkNumber(GetPosition()) };

	if ((directionToPlayer.y * directionToPlayer.y < 10) && directionToPlayer.x * sight.x > 0) {
		if (sameRoomCheck(player) == true) {
			return true;
		}
		else {
			//door state check
			if (sight.x < 0) {
				PM::GameObject* Lobj = world->GetRoom(this)->GetLeftObj();
				if (Lobj != nullptr && Lobj->GetGOComponent<PM::Collision>() == nullptr && world->GetRoom(this)->GetLeftRoom() == world->GetRoom(player)) {
					return true;
				}
			}
			else if (sight.x > 0) {
				PM::GameObject* Robj = world->GetRoom(this)->GetRightObj();
				if (Robj != nullptr && Robj->GetGOComponent<PM::Collision>() == nullptr && world->GetRoom(this)->GetRightRoom() == world->GetRoom(player)) {
					return true;
				}
			}
		}

	}
	return false;
}

NPC_generator::NPC_generator(PM::GameObject* p):player(p)
{
}

void NPC_generator::setting(EntityType t, NPCLevelList l, bool isControlled_)
{
	type = t;
	level = l;
	isControlled = isControlled_;
}

void NPC_generator::settingType(EntityType t)
{
	type = t;
}

void NPC_generator::settingLevel(NPCLevelList l)
{
	level = l;
}

void NPC_generator::settingIsControlled(bool isControlled_)
{
	isControlled = isControlled_;
}

Basic_NPC* NPC_generator::generate(std::string name, glm::vec2 position/*, std::initializer_list<PM::Component*> taskAIs*/)
{
	Basic_NPC* npcPtr{ new Basic_NPC{player,name,position,type,level,isControlled} };
	
	switch (type)
	{
	case EntityType::Basic_NPC:
		break;
	case EntityType::Worker_NPC:
		npcPtr->setAI(new NPC_Worker_AI{ npcPtr });
		break;
	case EntityType::Enemy_NPC:
		npcPtr->setAI(new Enemy_soldier_AI{ npcPtr });
		break;
	case EntityType::Target_NPC:
		break;
	default:
		break;
	}
	
	//for (auto& t:taskAIs) {
	//	npcPtr->AddTaskAI(t);
	//}

	return npcPtr;
}
