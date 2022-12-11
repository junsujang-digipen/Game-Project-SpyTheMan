/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.h
Project: CS230
Author: Kevin Wright
Creation date: 2/14/2021
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include <glm/glm.hpp>	
#include "TransformMatrix.h"
#include "Entity.h"



namespace PM {
	class Component;
	class ObjData;
	class Sequence_base;

	class Texture;

	class GameObject : public Entity{
		public:
		GameObject() = default;
		GameObject(std::string name, glm::vec2 position = { 0, 0 },glm::vec2 size = {100,100}, std::string imageFilepath = "nothing");
		virtual ~GameObject();

		bool ShouldUpdateChunk() const { return updateChunk; }
		void SetAvailable(bool available) { Available = available; }
		bool IsAvailable() const { return Available; }
		bool Isinterrupted() const { return interrupted; }
		void SetInterrupted(bool inter)
		{
			interrupted = inter;
		}
		void UpdateChunkFinished() { updateChunk = false; }
		
		void Update(double dt) override;
		void Draw(math::TransformMatrix& displayMatrix) const override;
		
		virtual void UpdatePosition(glm::vec2 addPosition) final { Entity::UpdatePosition(addPosition); updateChunk = true; }
		virtual void UpdateVelocity(glm::vec2 newVelocity) final { Entity::UpdateVelocity(newVelocity); updateChunk = true;}
		virtual void SetVelocity(glm::vec2 newVelocity) final { Entity::SetVelocity(newVelocity); updateChunk = true;}
		virtual void SetPosition(glm::vec2 newPosition) final { Entity::SetPosition(newPosition); updateChunk = true;}
		void SetLayer(int layer);
		int GetLayer() const;
		
		Sequence_base* GetNowSequence() { return nowSequence; }
		void putSequence(Sequence_base* sq) { nowSequence = sq; }
		void ChangeImage(std::string filepath);
	protected:
		class State {
		public:
			virtual void Enter(GameObject* object) = 0;
			virtual void Update(GameObject* object, double dt) = 0;
			virtual void TestForExit(GameObject* object) = 0;
			virtual std::string GetName() = 0;
		};
		class State_Nothing : public State {
		public:
			void Enter(GameObject*) override {}
			void Update(GameObject*, double) override {}
			void TestForExit(GameObject*) override {}
			std::string GetName() override { return ""; }
		};
		State_Nothing state_nothing;
	


		void ChangeState(State* newState);
		State* currState;
		
	private:

		bool updateChunk;
		int layer{ 1 };
		bool destroyObject;
		bool Available;
		bool interrupted{false};
	
		Sequence_base* nowSequence{nullptr};
		PM::Texture* imagePtr{nullptr};
	};

}
