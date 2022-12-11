#include "Entity.h"
#include "Engine.h"
#include "Collision.h"
#include "Sprite.h"
#include "ShowCollision.h"


PM::Entity::Entity(std::string name, glm::vec2 position, glm::vec2 size, glm::vec2 velocity)
	: name(name), position(position), velocity(velocity), size(size), updateMatrix(true), faceleft(false)
{

}

PM::Entity::~Entity()
{
	if (ParentObj != nullptr)
	{
		ParentObj->ExceptChildObj(this);
	}
	ClearGOComponents();
}

void PM::Entity::Update(double dt)
{
	glm::vec2 Vel = GetVelocity();
	if (Vel.x != 0 || Vel.y != 0) {
		UpdatePosition(Vel * glm::vec2{ dt,dt });
	}
	UpdateGOComponents(dt);
}

void PM::Entity::Draw(math::TransformMatrix& displayMatrix) const
{	
	Sprite* spritePtr = GetGOComponent<Sprite>();
	if (spritePtr != nullptr) {
		glm::mat3 S = { {GetSize().x / 2, 0, 0}, {0, GetSize().y / 2, 0}, {0, 0, 1} };
		glm::mat3 T = { {1, 0, 0}, {0, 1, 0}, {GetPosition().x + GetSize().x / 2, GetPosition().y + GetSize().y / 2, 1} };
		if (faceleft == true) {
			T = { {-1, 0, 0}, {0, 1, 0}, {GetPosition().x + GetSize().x / 2, GetPosition().y + GetSize().y / 2, 1} };
		}
		spritePtr->Draw(displayMatrix * T * S);
	}
	ShowCollision* showCollisionPtr = Engine::GetGSComponent<ShowCollision>();
	if (showCollisionPtr != nullptr && showCollisionPtr->IsEnabled() == true) {
		Collision* collisionPtr = GetGOComponent<Collision>();
		if (collisionPtr != nullptr) {
			collisionPtr->Draw(displayMatrix);
		}
	}
}


bool PM::Entity::DoesCollideWith(PM::Entity* objectB) {
	return GetGOComponent<Collision>() != nullptr && objectB->GetGOComponent<Collision>() != nullptr
		&& GetGOComponent<Collision>()->DoesCollideWith(objectB);
}

bool PM::Entity::DoesCollideWith(glm::vec2 point) {
	return GetGOComponent<Collision>() != nullptr && GetGOComponent<Collision>()->DoesCollideWith(point);
}

void PM::Entity::UpdateCollision(PM::Entity* obj) {

	//Engine::GetLogger().LogError("Base class collision resolution should not be called");
	std::string objName{ obj->name };
	if (DoesCollideWith(obj) == true) {
		//Engine::GetLogger().LogEvent("Collision Detected: " + GetName() + " and " + obj->GetName());
		if (nowCollision.find(objName) == nowCollision.end()/*nowCollision[objName] == false, Does this check need?*/) {
			CollisionEnter(obj);
			CollisionStay(obj);
			nowCollision[objName] = true;
		}
		else if (nowCollision[objName] == true) {//?
			CollisionStay(obj);
		}

	}
	else if (nowCollision.find(objName) != nowCollision.end()) {
		if (nowCollision[objName] == true) {
			CollisionExit(obj);
			nowCollision[objName] = false;
			nowCollision.erase(nowCollision.find(objName));
		}
	}
}


bool PM::Entity::CanCollideWith(EntityType objectBType) {
	if (static_cast<int>(objectBType) == 0)
	{
		return true;
	}
	return false;
}


PM::Entity* PM::Entity::GetChildObj(std::string n) const
{
	for(Entity* entity : childObjects)
	{
		if(entity->GetName() == n)
		{
			return entity;
		}
	}
	return nullptr;
}

template <typename T>
T* PM::Entity::GetChildObj() const
{
	for (Entity* entity : childObjects) {
		T* ptr = dynamic_cast<T*>(entity);
		if (ptr != nullptr) {
			return ptr;
		}
	}
	return nullptr;
}

template <typename T>
void PM::Entity::ExceptChildObj()
{
	auto it = std::find_if(childObjects.begin(), childObjects.end(), [](Entity* element) {
		return (dynamic_cast<T*>(element) != nullptr);
		});
	childObjects.erase(it);
}

void PM::Entity::ExceptChildObj(Entity* obj)
{
	auto it = std::find(childObjects.begin(), childObjects.end(), obj);
	if (it != childObjects.end())
	{
		childObjects.erase(it);
	}
}

void PM::Entity::ExceptChildObj(std::string n)
{
	auto it = std::find_if(childObjects.begin(), childObjects.end(), [n](Entity* element) {
		return element->GetName() == n;
		});
	childObjects.erase(it);
}

