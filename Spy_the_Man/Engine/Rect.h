/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Rect.h
Project: CS230
Author: Kevin Wright
Creation date: 2/12/2021
-----------------------------------------------------------------*/
#pragma once
#include <utility>
#include <glm/glm.hpp>

namespace math {
    struct [[nodiscard]] rect2 {
        glm::vec2 point1{ 0.0, 0.0 };
        glm::vec2 point2{ 0.0, 0.0 };

        constexpr glm::vec2 Size() const noexcept { return { Right() - Left(), Top() - Bottom() }; }
        constexpr double Left() const noexcept { return std::min(point1.x, point2.x); }     // if the rect was flipped, then bottomLeft.x > topRight.x
        constexpr double Right() const noexcept { return std::max(point1.x, point2.x); }    // if the rect was flipped, then bottomLeft.x > topRight.x
        constexpr double Top() const noexcept { return std::max(point1.y, point2.y); }   // if the rect uses top left as {0, 0} or was flipped in the y direction
        constexpr double Bottom() const noexcept { return std::min(point1.y, point2.y); }   // if the rect uses top left as {0, 0} or was flipped in the y direction
        constexpr glm::vec2 bottomLeft() const noexcept { return { Left(),Bottom() }; }
        constexpr glm::vec2 topRight() const noexcept { return { Right(),Top() }; }
    };
    struct [[nodiscard]] irect2 {
        glm::ivec2 point1{ 0, 0 };
        glm::ivec2 point2{ 0, 0 };

        constexpr glm::ivec2 Size() const noexcept { return { Right() - Left(), Top() - Bottom() }; }
        constexpr int Left() const noexcept { return std::min(point1.x, point2.x); }     // if the rect was flipped, then bottomLeft.x > topRight.x
        constexpr int Right() const noexcept { return std::max(point1.x, point2.x); }    // if the rect was flipped, then bottomLeft.x > topRight.x
        constexpr int Top() const noexcept { return std::max(point1.y, point2.y); }   // if the rect uses top left as {0, 0} or was flipped in the y direction
        constexpr int Bottom() const noexcept { return std::min(point1.y, point2.y); }   // if the rect uses top left as {0, 0} or was flipped in the y direction
    };
}
