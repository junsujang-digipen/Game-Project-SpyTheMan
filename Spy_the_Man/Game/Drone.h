/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Drone.h
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 3/5/2022
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\GameObject.h"
#include "..\Engine\Input.h"
#include "..\Engine\Rect.h"

class Drone : public PM::GameObject {
public:
    Drone(glm::vec2 position);
    void Update(double dt) override;
    void Draw(math::TransformMatrix& displayMatrix) const override;
    void CollisionEnter(Entity*) override;
    void CollisionStay(PM::Entity* obj) override;
    bool CanCollideWith(EntityType objectBType) override;
    
    EntityType GetEntityType() const override { return EntityType::Drone; }
    std::string GetEntityTypeName() const override { return "Drone"; }

    glm::vec2 GetCollisionSize() { return collisionSize.Size(); }

    void SetFinish(bool isfinish) { isFinish = isfinish; }
    bool IsFinish() { return isFinish; }

    void SetXVel(int value) { xVel = value; }
private:
    bool isFinish;

    DisguiseList disguisenow{};

    glm::vec2 droneSize{ 50, 50 };
    glm::vec2 camPos{};
    glm::vec2 idlePos{};
    math::irect2 collisionSize{ {0, 0}, {50, 50} };

    static inline double xVel = 500;

    float deadtime;

    PM::InputKey moveLeftKey;
    PM::InputKey moveRightKey;
    PM::InputKey finishKey;

    class State_Idle : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };
    class State_Moving : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Running"; }
    };
    class State_Finish : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Falling"; }
    };
    class State_Dead : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Falling"; }
    };
    State_Idle stateIdle;
    State_Moving stateMoving;
    State_Finish stateFinish;
    State_Dead stateDead;
};