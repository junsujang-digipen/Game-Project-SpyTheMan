/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Collision.cpp
Project: CS230
Author: Kevin Wright
Creation date: 2/12/2021
-----------------------------------------------------------------*/

#include "Engine.h"

#include "TransformMatrix.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include "Rect.h"
#include "Collision.h"
#include "Poodle.h"

bool LineAndLineCollisionCheck(glm::vec2 AStPoint, glm::vec2 ADirect, double Alength, glm::vec2 BStPoint, glm::vec2 BDirect, double Blength) {
    const glm::vec3 AtoBLineSt{ BStPoint - AStPoint,0 };
    const glm::vec3 AtoBLineEnd{ BStPoint + BDirect*(float)Blength - AStPoint,0 };
    const glm::vec3 ADR{ ADirect,0 };

    glm::vec3 res1 = glm::cross(ADR, AtoBLineSt);
    glm::vec3 res2 = glm::cross(ADR, AtoBLineEnd);

    if (res1.z > 0 && res2.z < 0 || res1.z < 0 && res2.z > 0) {
        const glm::vec3 BtoALineSt{ AStPoint - BStPoint ,0 };
        const glm::vec3 BtoALineEnd{ AStPoint + ADirect*(float)Alength - BStPoint ,0 };

        res1 = glm::cross(glm::vec3{ BDirect,0 }, BtoALineSt);
        res2 = glm::cross(glm::vec3{ BDirect,0 }, BtoALineEnd);
        if (res1.z > 0 && res2.z < 0 || res1.z < 0 && res2.z > 0) {
            return true;
        }
    }
    return false;
}
bool RectAndLineCollisionCheck(math::rect2 Arect, glm::vec2 BStPoint,glm::vec2 BDirect, double length) {
    const glm::vec2 RectPoint[5]{ glm::vec2{Arect.Top(),Arect.Left()},
        glm::vec2{Arect.Top(),Arect.Right()},
        glm::vec2{Arect.Bottom(),Arect.Left()},
        glm::vec2{Arect.Bottom(),Arect.Right()},
        glm::vec2{Arect.Top(),Arect.Left()}
    };
    const glm::vec2 linePoint[2]{BStPoint,BStPoint + BDirect * static_cast<float>(length) };

    constexpr int rectPointNum{4};
    for (int i = 0;i < rectPointNum;++i) {
        
        if (LineAndLineCollisionCheck(RectPoint[i], RectPoint[i + 1] - RectPoint[i], 1, BStPoint, BDirect, length) == true) return true;

        /*const glm::vec3 toLineSt{ glm::vec2{linePoint[0] - RectPoint[i]},0 };
        const glm::vec3 toLineEnd{ glm::vec2{linePoint[1] - RectPoint[i]},0 };
        const glm::vec3 rectCorner{ RectPoint[i+1] - RectPoint[i],0};

        glm::vec3 res1 = glm::cross(rectCorner, toLineSt);
        glm::vec3 res2 = glm::cross(rectCorner, toLineEnd);

        if (res1.z > 0 && res2.z <0 || res1.z < 0 && res2.z > 0) {
            const glm::vec3 toRectPointSt{ glm::vec2{RectPoint[i] - linePoint[0] },0 };
            const glm::vec3 toRectPointEnd{ glm::vec2{RectPoint[i+1] - linePoint[0] },0 };

            res1 = glm::cross(glm::vec3{ BDirect,0 }, toRectPointSt);
            res2 = glm::cross(glm::vec3{ BDirect,0 }, toRectPointEnd);
            if (res1.z > 0 && res2.z < 0 || res1.z < 0 && res2.z > 0) {
                return true;
            }
        }*/
    }
    return false;
}
bool CircleAndLineCollisionCheck(glm::vec2 ACenter, double radius, glm::vec2 BStPoint, glm::vec2 BDirect, double length) {
    
    /*const double m{ BDirect.y/ BDirect.x};
    const double mmxpy{-m*BStPoint.x + BStPoint.y};
    const double a{1 + m*m};
    const double b{-2*ACenter.x -2*ACenter.y*m + 2*m* mmxpy};
    const double c{ACenter.x*ACenter.x + ACenter.y * ACenter.y 
        + mmxpy*mmxpy -2*ACenter.y * mmxpy*mmxpy - radius*radius };
    const double D{b*b -4*a*c};*/
    const glm::vec2 BtoA{ BStPoint - ACenter };
    const double A{glm::dot(BDirect,BDirect)};
    const double B{2 * glm::dot(BDirect,BtoA)};
    const double C{glm::dot(BtoA,BtoA) - radius*radius};
    const double D{ B * B - 4 * A * C };
    
    if (D== 0) {
        const double t{(-B)/2*A};
        if (t<=length && t>=0) {
            return true;
        }
    }
    else if (D>0) {
        const double sqrtD{sqrt(D)};
        const double t1{ (-B + sqrtD) / 2 * A };
        const double t2{ (-B - sqrtD) / 2 * A };

        if (t1 <= length && t1 >= 0 || t2 <= length && t2 >= 0) {
            return true;
        }

        const double distSquare = BtoA.x * BtoA.x + BtoA.y * BtoA.y;
        const double collideDistSquare = radius * radius;
        if (collideDistSquare > distSquare) {
            return true;
        }
    }

    return false;
}

bool RectAndCircleCollisionCheck(math::rect2 rectA, glm::vec2 Bposition, double Bradius) {
    
    Bradius /= 2;
    
    if (Bposition.x < rectA.Left() && Bposition.y > rectA.Top()) {
        const double dist{ glm::distance(Bposition, glm::vec2{rectA.Left(),rectA.Top() }) };
        if (dist < Bradius) return true;
    }
    else if (Bposition.x < rectA.Left() && Bposition.y < rectA.Bottom()) {
        const double dist{ glm::distance(Bposition, glm::vec2{rectA.Left(),rectA.Bottom() }) };
        if (dist < Bradius) return true;
    }
    else if (Bposition.x > rectA.Right() && Bposition.y > rectA.Top()) {
        const double dist{ glm::distance(Bposition, glm::vec2{rectA.Right(),rectA.Top() }) };
        if (dist < Bradius) return true;
    }
    else if (Bposition.x > rectA.Right() && Bposition.y < rectA.Bottom()) {
        const double dist{ glm::distance(Bposition, glm::vec2{rectA.Right(),rectA.Bottom() }) };
        if (dist < Bradius) return true;
    }
    else {
        if (Bposition.x > rectA.Left() - Bradius && Bposition.x < rectA.Right() + Bradius
            && Bposition.y <rectA.Top() + Bradius && Bposition.y > rectA.Bottom() - Bradius
            /*|| Bposition.y <rectA.Top() && Bposition.y > rectA.Bottom()
            && Bposition.x > rectA.Left() - Bradius && Bposition.x < rectA.Right() + Bradius*/) {
            return true;
        }
    }

    return false;
}

void PM::RectCollision::Draw(math::TransformMatrix cameraMatrix) {
    PM::push_settings();
    PM::no_fill();
    PM::set_line_width(5);
    PM::apply_matrix(cameraMatrix.matrix);
    //Engine::GetLogger().LogDebug("Show collision draw");
    PM::draw_rect(GetWorldCoorRect().Left(), GetWorldCoorRect().Bottom(), GetWorldCoorRect().Size().x, GetWorldCoorRect().Size().y);
    PM::pop_settings();
}

void PM::CircleCollision::Draw(math::TransformMatrix cameraMatrix) {
    PM::push_settings();
    PM::no_fill();
    PM::set_line_width(5);
    PM::apply_matrix(cameraMatrix.matrix);
   // Engine::GetLogger().LogDebug("Show collision draw");
    PM::draw_ellipse(objectPtr->GetPosition().x, objectPtr->GetPosition().y, radius, radius);
    PM::pop_settings();
}

math::rect2 PM::RectCollision::GetWorldCoorRect() {
    return { glm::vec2{rect.point1.x + objectPtr->GetPosition().x, rect.point1.y + objectPtr->GetPosition().y },
        glm::vec2{rect.point2.x + objectPtr->GetPosition().x, rect.point2.y + objectPtr->GetPosition().y } };
}

void PM::RectCollision::SetCollision(math::irect2 newRect)
{
    rect = newRect;
}

bool PM::RectCollision::DoesCollideWith(Entity* objectB) {
    Collision* collideAgainst = objectB->GetGOComponent<Collision>();
    if (collideAgainst == nullptr) {
        Engine::GetLogger().LogError("testAgainstObject collision is null");
        return false;
    }
    PM::Collision::CollideType againstType = collideAgainst->GetCollideType();
    if (againstType != CollideType::Rect_Collide && againstType != CollideType::Circle_Collide && againstType != CollideType::Line_Collide) {
        Engine::GetLogger().LogError("Rect vs unsupported type");
        return false;
    }
    const math::rect2 rectA = GetWorldCoorRect();
    math::rect2 rectB{};
    glm::vec2 Bposition{};
    glm::vec2 Bdirection{};
    
    switch (againstType)
    {
    case PM::Collision::CollideType::Rect_Collide: 
        rectB = dynamic_cast<RectCollision*>(collideAgainst)->GetWorldCoorRect();
        if (rectA.Right() > rectB.Left() && rectA.Left() < rectB.Right() &&
            rectA.Bottom() < rectB.Top() && rectA.Top() > rectB.Bottom()) {
            return true;
        }
        break;
    case PM::Collision::CollideType::Circle_Collide:
        return RectAndCircleCollisionCheck(rectA,objectB->GetPosition(), dynamic_cast<CircleCollision*>(collideAgainst)->GetRadius());
        break;
    case PM::Collision::CollideType::Line_Collide:
        Bposition = { objectB->GetPosition() + dynamic_cast<LineCollision*>(collideAgainst)->getStPosition() };
        Bdirection = dynamic_cast<LineCollision*>(collideAgainst)->getDirection();
        return RectAndLineCollisionCheck(rectA,Bposition, Bdirection, dynamic_cast<LineCollision*>(collideAgainst)->getLength());

        break;
    default:
        break;
    }
    
    
    return false;
}

bool PM::RectCollision::DoesCollideWith(glm::vec2 point) {
    math::rect2 worldRect = GetWorldCoorRect();
    return point.x >= worldRect.Left() && point.x <= worldRect.Right() && point.y <= worldRect.Top() && point.y >= worldRect.Bottom();
}

double PM::CircleCollision::GetRadius() {
    return radius;
}

void PM::CircleCollision::SetCollision(double newRadius)
{
    radius = newRadius;
}

bool PM::CircleCollision::DoesCollideWith(Entity* objectB) {
    Collision* collideAgainst = objectB->GetGOComponent<Collision>();
    if (collideAgainst == nullptr) {
        Engine::GetLogger().LogError("testAgainstObject collision is null");
        return false;
    }
    PM::Collision::CollideType againstType = collideAgainst->GetCollideType();

    if (againstType != CollideType::Rect_Collide && againstType != CollideType::Circle_Collide && againstType != CollideType::Line_Collide) {
        Engine::GetLogger().LogError("Circle vs unsupported type");
        return false;
    }

    glm::vec2 Bposition{};
    glm::vec2 Bdirection{};

    glm::vec2 dist;
    double radiusA, radiusB, distSquare, collideDistSquare;

    switch (againstType) {
    case PM::Collision::CollideType::Rect_Collide:
        // Functionalize rect,circle collision check in a RectCollision function
        return RectAndCircleCollisionCheck(dynamic_cast<RectCollision*>(collideAgainst)->GetWorldCoorRect(), objectPtr->GetPosition(), radius);
        break;
    case PM::Collision::CollideType::Circle_Collide:
         dist = objectPtr->GetPosition() - objectB->GetPosition();
         radiusA = GetRadius();
         radiusB = dynamic_cast<CircleCollision*>(collideAgainst)->GetRadius();
         distSquare = dist.x * dist.x + dist.y * dist.y;
         collideDistSquare = (radiusA + radiusB) * (radiusA + radiusB);

        return distSquare < collideDistSquare;
        break;
    case PM::Collision::CollideType::Line_Collide:
        Bposition = { objectB->GetPosition() + dynamic_cast<LineCollision*>(collideAgainst)->getStPosition() };
        Bdirection = dynamic_cast<LineCollision*>(collideAgainst)->getDirection();
        return CircleAndLineCollisionCheck(objectPtr->GetPosition(), GetRadius(), Bposition,Bdirection, dynamic_cast<LineCollision*>(collideAgainst)->getLength());

        break;
    default:
        break;
    }

    return false;
}

bool PM::CircleCollision::DoesCollideWith(glm::vec2 point) {

    glm::vec2 dist = objectPtr->GetPosition() - point;
    double distSquare = dist.x * dist.x + dist.y * dist.y;
    double radiusA = GetRadius();
    double collideDistSquare = radiusA * radiusA;

    return distSquare < collideDistSquare;
}

void PM::LineCollision::Draw(math::TransformMatrix /*displayMatrix*/)
{
}

bool PM::LineCollision::DoesCollideWith(Entity* objectB)
{
    Collision* collideAgainst = objectB->GetGOComponent<Collision>();
    if (collideAgainst == nullptr) {
        Engine::GetLogger().LogError("testAgainstObject collision is null");
        return false;
    }
    PM::Collision::CollideType againstType = collideAgainst->GetCollideType();

    if (againstType != CollideType::Rect_Collide && againstType != CollideType::Circle_Collide && againstType != CollideType::Line_Collide) {
        Engine::GetLogger().LogError("Line vs unsupported type");
        return false;
    }
    switch (againstType) {
    case PM::Collision::CollideType::Rect_Collide:
        return RectAndLineCollisionCheck(dynamic_cast<RectCollision*>(collideAgainst)->GetWorldCoorRect(), stPosition,directtion, length);
        break;
    case PM::Collision::CollideType::Circle_Collide:
        return CircleAndLineCollisionCheck(objectB->GetPosition(), dynamic_cast<CircleCollision*>(collideAgainst)->GetRadius(), stPosition, directtion, length);

        break;
    case PM::Collision::CollideType::Line_Collide:
        return LineAndLineCollisionCheck(stPosition, directtion, length,
            dynamic_cast<LineCollision*>(collideAgainst)->getStPosition(), 
            dynamic_cast<LineCollision*>(collideAgainst)->getDirection(), 
            dynamic_cast<LineCollision*>(collideAgainst)->getLength());

        break;
    default:
        break;
    }
    return false;
}

glm::vec2 PM::LineCollision::getDirection()
{
    return directtion;
}

glm::vec2 PM::LineCollision::getStPosition()
{
    return stPosition;
}

double PM::LineCollision::getLength()
{
    return length;
}

void PM::LineCollision::SetCollision(glm::vec2 newStPoint, glm::vec2 newDirecttion, double newLength) {
    stPosition = newStPoint;
    directtion = newDirecttion;
    length = newLength;
}
