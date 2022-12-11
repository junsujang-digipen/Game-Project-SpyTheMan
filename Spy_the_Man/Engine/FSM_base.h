#pragma once
/*--------------------------------------------------------------
File Name: FSM_base.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 20/9/2021
-----------------------------------------------------------------*/
#include <string>

namespace PM {
	template <typename ownerObj>
	class FSM_base {
	protected:
		class State {
		public:
			virtual void Enter(ownerObj* object) = 0;
			virtual void Update(ownerObj* object, double dt) = 0;
			virtual void Draw(ownerObj* /*object*/) {}
			virtual void TestForExit(ownerObj* object) = 0;
			//virtual std::string GetTitle() = 0;
			virtual std::string GetName() = 0;
			virtual ~State() {}
		};
		class State_Nothing : public State {
		public:
			void Enter(ownerObj*) override {}
			void Update(ownerObj*, double) override {}
			void TestForExit(ownerObj*) override {}
			std::string GetName() override { return "State_Nothing"; }
		};
		
		State* currState;
		State_Nothing stateNothing;
		
		void ChangeState(State* newState) {
			currState = newState;
			currState->Enter(static_cast<ownerObj*>(this));
		}

	public:
		virtual ~FSM_base()
		{

		}
	};
}