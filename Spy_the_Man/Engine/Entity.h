#pragma once
#include <map>
#include <string>
#include "ComponentManager.h"
#include "Declaration.h"
#include "TransformMatrix.h"

namespace PM
{
	// value -> data -> game
	// game -/> data -> value
	class Component;
	class ObjData;
	class Entity
	{
	private:
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec2 size;

		std::string name{ "Empty" };
		EntityType entityType{EntityType::Empty};
		math::TransformMatrix objectMatrix;
		Entity* ParentObj{ nullptr };
		std::vector<Entity*> childObjects;
		
		PM::ComponentManager components;
		std::map<std::string, bool> nowCollision{};

		bool updateMatrix;
		bool faceleft;
		bool destroyObject;
	public:
		Entity() = default;
		Entity(std::string name, glm::vec2 position, glm::vec2 size = { 100,100 },
			glm::vec2 velocity = { 0,0 });
		virtual ~Entity();
		virtual void Update([[maybe_unused]] double dt);
		virtual void Draw([[maybe_unused]] math::TransformMatrix& displayMatrix) const;
		virtual void FrontDraw([[maybe_unused]] math::TransformMatrix& displayMatrix) const {}
		virtual void UpdatePosition(glm::vec2 addPosition) { position += addPosition; }
		virtual void UpdateVelocity(glm::vec2 newVelocity) { velocity += newVelocity; }
		virtual void SetSize(glm::vec2 newSize) { size = newSize; }
		virtual void SetVelocity(glm::vec2 newVelocity) { velocity = newVelocity; }
		virtual void SetPosition(glm::vec2 newPosition) { position = newPosition; }
		const glm::vec2& GetPosition() const { return position; }
		const glm::vec2& GetVelocity() const { return velocity; }
		const glm::vec2& GetSize() const { return size; }
		const glm::vec2 GetCenter() const { return position + glm::fvec2{ size.x / 2, size.y / 2 }; }
		std::string GetName() const { return name; }
		virtual ObjData* MakeDataFile() const { return nullptr; }

		virtual bool CanCollideWith(EntityType objectBType);
		bool DoesCollideWith(Entity* objectB);
		bool DoesCollideWith(glm::vec2 point);
		void UpdateCollision(Entity*);
		virtual void CollisionEnter(Entity*) {}
		virtual void CollisionStay(Entity*) {}
		virtual void CollisionExit(Entity*) {}
		virtual EntityType GetEntityType() const { return EntityType::Empty; }
		virtual std::string GetEntityTypeName() const { return "Empty"; }

		virtual DisguiseList GetDisguise() const { return DisguiseList::Player; }

		void FaceLeft(bool left) { faceleft = left; }
		bool IsFaceLeft() const { return faceleft; }
		bool IsUpdateMatrix() const { return updateMatrix; }
		void Destroy() { destroyObject = true; }
		bool ShouldDestroy() const { return destroyObject; }
		
		Entity* GetParentObj() const { return ParentObj; }
		Entity* GetChildObj(std::string name) const;

		template<typename T>
		T* GetChildObj() const;

		template<typename T>
		void ExceptChildObj();
		void ExceptChildObj(Entity* obj);
		void ExceptChildObj(std::string name);
		
		template<typename T>
		T* GetGOComponent() const { return components.GetComponent<T>(); }
		template<typename T>
		void RemoveGOComponent() { components.RemoveComponent<T>(); }

		void AddGOComponent(Component* component) { components.AddComponent(component); }
	protected:
		void UpdateGOComponents(double dt) { components.UpdateAll(dt); }
		void ClearGOComponents() { components.Clear(); }

	};
}