#pragma once

#include "Vec3.h"

namespace Pelvis
{

struct Mat4
{
    float data[16];

    Mat4();

    static Mat4 identity();

    static Mat4 perspective(
        float fovDegrees,
        float aspect,
        float nearPlane,
        float farPlane
    );

    static Mat4 lookAt(
        const Vec3& position,
        const Vec3& target,
        const Vec3& up
    );

    Mat4 operator*(const Mat4& other) const;
};

}
