/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Collision.h
Project: CS230
Author: Kevin Wright
Creation date: 2/12/2021
-----------------------------------------------------------------*/
#pragma once


#include "Component.h"
#include "Rect.h"

namespace glm {
    class TransformMatrix;
}

namespace PM {
    class GameObject;

    class Collision : public Component {
        std::map<std::string, bool> a;
    public:

        enum class CollideType {
            Rect_Collide,
            Circle_Collide,
            Line_Collide,
        };
        Collision() : Component("Collision") {}
        Collision(std::string n) : Component(n) {}
        virtual void Draw(math::TransformMatrix displayMatrix) = 0;
        virtual CollideType GetCollideType() = 0;

        virtual bool DoesCollideWith(Entity* gameObject) = 0;
        virtual bool DoesCollideWith(glm::vec2 point) = 0;
    };

    class RectCollision : public Collision {
    public:
        RectCollision(math::irect2 rect, Entity* objectPtr) : Collision("RectCollision"),
    	rect(rect), objectPtr(objectPtr) {}
        void Draw(math::TransformMatrix displayMatrix) override;
        CollideType GetCollideType() override { return Collision::CollideType::Rect_Collide; };
        bool DoesCollideWith(Entity* objectB) override;
        bool DoesCollideWith(glm::vec2 point) override;
        math::rect2 GetWorldCoorRect();
        void SetCollision(math::irect2 newRect);
    private:
        Entity* objectPtr;
        math::irect2 rect;
    };

    class CircleCollision : public Collision {
    public:
        CircleCollision(double radius, Entity* objectPtr) : Collision("CircleCollision"),
    	radius(radius), objectPtr(objectPtr) {}
        void Draw(math::TransformMatrix displayMatrix) override;
        CollideType GetCollideType() override { return Collision::CollideType::Circle_Collide; };

        bool DoesCollideWith(Entity* objectB) override;
        bool DoesCollideWith(glm::vec2 point) override;
        double GetRadius();
        void SetCollision(double radius);
    private:
        Entity* objectPtr;
        double radius;
    };

    class LineCollision : public Collision {
    public:
        LineCollision(glm::vec2 stPoint,glm::vec2 directtion,double length, GameObject* objectPtr) : Collision("LineCollision"),
            stPosition(stPoint), directtion(directtion),length(length), objectPtr(objectPtr) {}

        void Draw(math::TransformMatrix displayMatrix) override;
        CollideType GetCollideType() override { return Collision::CollideType::Line_Collide; };

        bool DoesCollideWith(Entity* objectB) override;
        //bool DoesCollideWith(glm::vec2 point) override;
        glm::vec2 getDirection();
        glm::vec2 getStPosition();
        double getLength();
        void SetCollision(glm::vec2 newStPoint, glm::vec2 newDirecttion, double newLength);
    private:
        GameObject* objectPtr;
        glm::vec2 directtion;
        glm::vec2 stPosition;
        double length;
    };
}