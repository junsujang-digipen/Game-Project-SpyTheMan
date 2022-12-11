/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: TransformMatrix.h
Project: CS230
Author: Kevin Wright
Creation date: 2/12/2021
-----------------------------------------------------------------*/
#pragma once
#include <glm/glm.hpp>

namespace math {
    class TransformMatrix {
    public:
        TransformMatrix();
        TransformMatrix(glm::mat3 matrix) :matrix(matrix) {}
        TransformMatrix operator * (TransformMatrix rhs) const;
        TransformMatrix& operator *= (TransformMatrix rhs);
        glm::vec2 operator * (glm::vec2 rhs) const;
        void Reset();
        
    //protected:
        glm::mat3 matrix{};
    };

    class TranslateMatrix : public TransformMatrix {
    public:
        TranslateMatrix(glm::vec2 translate);
    };

    class RotateMatrix : public TransformMatrix {
    public:
        RotateMatrix(double radians);
    };

    class ScaleMatrix : public TransformMatrix {
    public:
        ScaleMatrix(glm::vec2 scale);
    };
}