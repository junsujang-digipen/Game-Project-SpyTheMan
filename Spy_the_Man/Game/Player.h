/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Player.h
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 10/7/2021
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\GameObject.h"
#include "..\Engine\Input.h"
#include "..\Engine\Rect.h"

namespace PM {
    class Sprite;
}

class Player : public PM::GameObject {
public:
    Player(glm::vec2 position);
    ~Player();
    void Update(double dt) override;
    void Draw(math::TransformMatrix& displayMatrix) const override;
    void CollisionEnter(Entity*) override;
    void CollisionStay(PM::Entity* obj) override;
    bool CanCollideWith(EntityType objectBType) override;

    void GoTo(glm::vec2 destination);
    void SetGoTo(bool g) { gotopos = g; }
    void SetHide(bool h) { hide = h; }
    void SetVent(bool v) { vent = v; }
    void SetDead(bool d) { if (invincible == false) { isDead = d; } }
    void SetTalk(bool t) { isTalk = t; }
    void SetDisguise(DisguiseList disguisejob);
    void SetPos(glm::vec2 pos);
    void SetInvincible() { invincible = true; }
    void SetDrone(bool d) { drone = d; }
    void SetSilent(bool s) { silent = s; }
    void SetCloset(bool c) { closet = c; }

    //skills
    void SetChameleon(bool cha) { chameleon = cha; }
    void SetInvisible(bool inv) { invisible = inv; }
    void SetDash(bool d) { dash = d; }
    void DrawTrap(bool draw, bool left = false) { drawtrap = draw, trapleft = left; }
    void DrawPush(bool draw, bool left = false) { drawpush = draw, pushleft = left; }

    EntityType GetEntityType() const override { return EntityType::Player; }
    std::string GetEntityTypeName() const override { return "Player"; }

    glm::vec2 GetCollisionSize() { return collisionSize.Size(); }

    bool IsDead() { return isDead; }
    bool IsHide() { return hide; }
    bool IsVent() { return vent; }
    bool IsTalk() { return isTalk; }
    bool IsDrone() { return drone; }
    bool IsDestroy() { return destroy; }
    bool IsCloset() { return closet; }
    DisguiseList GetDisguise() const override { return disguisenow; }
    glm::vec2 endPos{};
    bool GetGoToPos() { return gotopos; }

    void SetXVel(int value) { xVel = value; }
    bool GetCheckRun() { return run; }
private:
    std::string playername = "player";
    bool isDead;
    bool isTalk;
    bool drawPlayer;
    bool gotopos;
    bool hide;
    bool vent;
    bool invincible;
    bool run;
    bool runpressed;
    bool drone;
    bool silent;
    bool dashing;
    bool closet;
    bool destroy;
    double hidetime;
    double invtime = 4;
    double dashtime = 0.2;
    double distime = 15;
    double dashcounter = 5;

    //skils
    bool chameleon;
    bool invisible;
    bool dash;
    bool drawtrap;
    bool trapleft;
    bool drawpush;
    bool pushleft;

    DisguiseList disguisenow{};

    glm::vec2 playerSize{ 250, 250 };
    glm::vec2 runningPos{};
    glm::vec2 walkingPos{};
    math::irect2 collisionSize{ {70, 0}, {180, 200} };
    math::irect2 ventSize{ {20, 0}, {230, 100} };

    static inline double xVel = 300;

    PM::InputKey moveLeftKey;
    PM::InputKey moveRightKey;
    PM::InputKey runKey;
    PM::InputKey dashKey;

    PM::Sprite* dead;

    double alp = 275;
    double time = 0;

    double runtime = 0;
    bool runwave = false;

    class State_Idle : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    private:
        static constexpr double detectTime{3};
        double detectTimer{0};
    };
    class State_Moving : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Moving"; }
    };
    class State_MovingtoPos : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "MovingtoPos"; }
    };
    class State_Hiding : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Hiding"; }
    };
    class State_VentIdle : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "VentIdle"; }
    };
    class State_Venting : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Venting"; }
    };
    class State_VenttoPos : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "VenttoPos"; }
    };
    class State_Killing : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Killing"; }
    };
    class State_Dead : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Dead"; }
    };
    class State_Talk : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Talk"; }
    };
    class State_Dash : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Dash"; }
    };
    class State_Destroy : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Destroy"; }
    };
    State_Idle stateIdle;
    State_Moving stateMoving;
    State_MovingtoPos stateMovingtoPos;
    State_Hiding stateHiding;
    State_VentIdle stateVentIdle;
    State_Venting stateVenting;
    State_VenttoPos stateVenttoPos;
    State_Killing stateKilling;
    State_Dead stateDead;
    State_Talk stateTalk;
    State_Dash statedash;
    State_Destroy stateDestroy;
};