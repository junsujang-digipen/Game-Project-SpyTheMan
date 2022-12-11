#pragma once
/*--------------------------------------------------------------
File Name: AI.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 9/20/2021
-----------------------------------------------------------------*/
#include "Component.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "FSM_base.h"

namespace PM {
	//This class will put in to AI owner and make the owner to do task.
	template<typename ownerObj>
	class AI :public PM::Component,public FSM_base<ownerObj> {
	protected:
		PM::GameObject* AIOwner = nullptr;
		class State_start :public FSM_base<ownerObj>::State {
		public:
			void Enter(ownerObj*) override {}
			void Update(ownerObj*, double) override {}
			void TestForExit(ownerObj* ai) override { ai->start(); }
			std::string GetName() override { return "State_start"; }
		};
		State_start stateStart;
	public:
		AI(PM::GameObject* AIOwner) :AIOwner(AIOwner) { FSM_base<ownerObj>::ChangeState(&(FSM_base<ownerObj>::stateNothing)); }
		virtual void start() { }
		void setOwner(PM::GameObject* Owner) { AIOwner = Owner;start(); }
		//function takes taskAIcompo

		void Update(double dt)override {
			FSM_base<ownerObj>::currState->Update(static_cast<ownerObj*>(this), dt);
			//TaskAIComponents.UpdateAll(dt);
			FSM_base<ownerObj>::currState->TestForExit(static_cast<ownerObj*>(this));
		}

		~AI()override {  }
	};
	//와 실리콘 벨리 ㅈㄴ 재미있다
		//와 지려
		//
}