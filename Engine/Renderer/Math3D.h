#pragma once

#include <cmath>

namespace Pelvis
{

struct Vec3
{
    float x;
    float y;
    float z;
};

struct Mat4
{
    float m[16];

    static Mat4 identity()
    {
        Mat4 result{};

        result.m[0]  = 1.0f;
        result.m[5]  = 1.0f;
        result.m[10] = 1.0f;
        result.m[15] = 1.0f;

        return result;
    }
};

inline Mat4 perspective(
    float fovDegrees,
    float aspect,
    float nearPlane,
    float farPlane
)
{
    Mat4 result{};

    constexpr float PI = 3.14159265359f;

    const float radians =
        fovDegrees * PI / 180.0f;

    const float tanHalfFov =
        std::tan(radians * 0.5f);

    result.m[0] =
        1.0f / (aspect * tanHalfFov);

    result.m[5] =
        1.0f / tanHalfFov;

    result.m[10] =
        -(farPlane + nearPlane) /
        (farPlane - nearPlane);

    result.m[11] = -1.0f;

    result.m[14] =
        -(2.0f * farPlane * nearPlane) /
        (farPlane - nearPlane);

    return result;
}

inline Mat4 viewMatrix(
    const Vec3& position,
    float yawDegrees,
    float pitchDegrees
)
{
    constexpr float PI = 3.14159265359f;

    const float yaw =
        yawDegrees * PI / 180.0f;

    const float pitch =
        pitchDegrees * PI / 180.0f;

    const float cy = std::cos(yaw);
    const float sy = std::sin(yaw);

    const float cp = std::cos(pitch);
    const float sp = std::sin(pitch);

    Vec3 forward
    {
        cy * cp,
        sp,
        sy * cp
    };

    Vec3 right
    {
        -sy,
        0.0f,
        cy
    };

    Vec3 up
    {
        -cy * sp,
        cp,
        -sy * sp
    };

    Mat4 result{};

    result.m[0] = right.x;
    result.m[1] = up.x;
    result.m[2] = -forward.x;

    result.m[4] = right.y;
    result.m[5] = up.y;
    result.m[6] = -forward.y;

    result.m[8] = right.z;
    result.m[9] = up.z;
    result.m[10] = -forward.z;

    result.m[12] =
        -(right.x * position.x +
          right.y * position.y +
          right.z * position.z);

    result.m[13] =
        -(up.x * position.x +
          up.y * position.y +
          up.z * position.z);

    result.m[14] =
        forward.x * position.x +
        forward.y * position.y +
        forward.z * position.z;

    result.m[15] = 1.0f;

    return result;
}

}
