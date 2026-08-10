#include "Mat4.h"

#include <cmath>

namespace Pelvis
{

static float dot(
    const Vec3& a,
    const Vec3& b
)
{
    return
        a.x * b.x +
        a.y * b.y +
        a.z * b.z;
}

static Vec3 cross(
    const Vec3& a,
    const Vec3& b
)
{
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

static Vec3 normalize(const Vec3& v)
{
    float length =
        std::sqrt(
            v.x * v.x +
            v.y * v.y +
            v.z * v.z
        );

    if (length == 0.0f)
    {
        return Vec3();
    }

    return Vec3(
        v.x / length,
        v.y / length,
        v.z / length
    );
}

Mat4::Mat4()
{
    for (int i = 0; i < 16; ++i)
    {
        data[i] = 0.0f;
    }
}

Mat4 Mat4::identity()
{
    Mat4 result;

    result.data[0] = 1.0f;
    result.data[5] = 1.0f;
    result.data[10] = 1.0f;
    result.data[15] = 1.0f;

    return result;
}

Mat4 Mat4::perspective(
    float fovDegrees,
    float aspect,
    float nearPlane,
    float farPlane
)
{
    Mat4 result;

    constexpr float PI = 3.14159265359f;

    float radians =
        fovDegrees * PI / 180.0f;

    float tanHalfFov =
        std::tan(radians / 2.0f);

    result.data[0] =
        1.0f / (aspect * tanHalfFov);

    result.data[5] =
        1.0f / tanHalfFov;

    result.data[10] =
        -(farPlane + nearPlane) /
        (farPlane - nearPlane);

    result.data[11] = -1.0f;

    result.data[14] =
        -(2.0f * farPlane * nearPlane) /
        (farPlane - nearPlane);

    return result;
}

Mat4 Mat4::lookAt(
    const Vec3& position,
    const Vec3& target,
    const Vec3& up
)
{
    Vec3 forward =
        normalize(target - position);

    Vec3 right =
        normalize(cross(forward, up));

    Vec3 cameraUp =
        cross(right, forward);

    Mat4 result =
        Mat4::identity();

    result.data[0] = right.x;
    result.data[4] = right.y;
    result.data[8] = right.z;

    result.data[1] = cameraUp.x;
    result.data[5] = cameraUp.y;
    result.data[9] = cameraUp.z;

    result.data[2] = -forward.x;
    result.data[6] = -forward.y;
    result.data[10] = -forward.z;

    result.data[12] =
        -dot(right, position);

    result.data[13] =
        -dot(cameraUp, position);

    result.data[14] =
        dot(forward, position);

    return result;
}

Mat4 Mat4::operator*(
    const Mat4& other
) const
{
    Mat4 result;

    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            result.data[
                column * 4 + row
            ] =
                data[row] *
                other.data[column * 4]

                +

                data[4 + row] *
                other.data[column * 4 + 1]

                +

                data[8 + row] *
                other.data[column * 4 + 2]

                +

                data[12 + row] *
                other.data[column * 4 + 3];
        }
    }

    return result;
}

}
