/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: TransformMatrix.cpp
Project: CS230
Author: Sangbeom Kim
Creation date: 2/12/2021
-----------------------------------------------------------------*/
#include <cmath>   //std::sin & std::cos
#include "TransformMatrix.h"

math::TransformMatrix::TransformMatrix() {
    Reset();
}

void math::TransformMatrix::Reset() {
    matrix = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
}

math::TranslateMatrix::TranslateMatrix(glm::vec2 translate) {
    matrix = { {1, 0, 0}, {0, 1, 0}, {translate.x, translate.y, 1} };
}

math::RotateMatrix::RotateMatrix(double radians) {
    matrix = { {cos(radians), sin(radians), 0}, {-sin(radians), cos(radians), 0}, {0, 0, 1} };
}

math::ScaleMatrix::ScaleMatrix(glm::vec2 scale) {
    matrix = { {scale.x, 0, 0}, {0, scale.y, 0}, {0, 0, 1} };
}

math::TransformMatrix math::TransformMatrix::operator * (TransformMatrix rhs) const {
    TransformMatrix result;
    result.matrix = matrix * rhs.matrix;
    return result;
}

math::TransformMatrix& math::TransformMatrix::operator *= (math::TransformMatrix rhs) {
    (*this) = (*this) * rhs;
    return (*this);
}

glm::vec2 math::TransformMatrix::operator * (glm::vec2 rhs) const {
    glm::vec2 result;
    result.x = matrix[0][0] * rhs.x + matrix[0][1] * rhs.y + matrix[0][2];
    result.y = matrix[1][0] * rhs.x + matrix[1][1] * rhs.y + matrix[1][2];
    return result;
}