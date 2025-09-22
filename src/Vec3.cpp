#include "KumaGL/Vec3.hpp"

#include <cmath>

namespace KumaGL
{
    /******************************************************************************/
    Vec3::Vec3(float a, float b, float c) : x(a), y(b), z(c) {}

    /******************************************************************************/
    void Vec3::operator+=(const Vec3 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }

    /******************************************************************************/
    void Vec3::operator-=(const Vec3 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
    }

    /******************************************************************************/
    void Vec3::operator*=(float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
    }

    /******************************************************************************/
    void Vec3::Normalize() { (*this) = KumaGL::Normalize(*this); }

    /******************************************************************************/
    Vec3 Vec3::Cross(const Vec3 &aVec) const
    {
        return KumaGL::Cross((*this), aVec);
    }

    /******************************************************************************/
    Vec3 Vec3::Direction(const Vec3 &aVec) const
    {
        return KumaGL::Direction((*this), aVec);
    }

    /******************************************************************************/
    float Vec3::Dot(const Vec3 &aVec) const { return KumaGL::Dot((*this), aVec); }

    /******************************************************************************/
    float Vec3::Distance(const Vec3 &aVec) const
    {
        return KumaGL::Distance((*this), aVec);
    }

    /******************************************************************************/
    std::ostream &operator<<(std::ostream &os, const Vec3 &aVec)
    {
        os << aVec.x << " " << aVec.y << " " << aVec.z;
        return os;
    }

    /******************************************************************************/
    Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs)
    {
        return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    };

    /******************************************************************************/
    Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs)
    {
        return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    };

    /******************************************************************************/
    Vec3 operator*(const Vec3 &lhs, float rhs)
    {
        return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
    }

    /******************************************************************************/
    Vec3 Normalize(const Vec3 &aVector)
    {
        auto magnitude = std::sqrt(std::pow(aVector.x, 2) + std::pow(aVector.y, 2) +
                                   std::pow(aVector.z, 2));

        Vec3 result(0, 0, 0);
        if (magnitude > 0)
        {
            result.x = aVector.x / magnitude;
            result.y = aVector.y / magnitude;
            result.z = aVector.z / magnitude;
        }

        return result;
    }

    /******************************************************************************/
    Vec3 Cross(const Vec3 &aVectorA, const Vec3 &aVectorB)
    {
        auto yz = aVectorA.y * aVectorB.z;
        auto zy = aVectorA.z * aVectorB.y;
        auto zx = aVectorA.z * aVectorB.x;
        auto xz = aVectorA.x * aVectorB.z;
        auto xy = aVectorA.x * aVectorB.y;
        auto yx = aVectorA.y * aVectorB.x;

        return Vec3(yz - zy, zx - xz, xy - yx);
    }

    /******************************************************************************/
    Vec3 Direction(const Vec3 &aVectorA, const Vec3 &aVectorB)
    {
        Vec3 dir = aVectorB - aVectorA;
        dir.Normalize();

        return dir;
    }

    /******************************************************************************/
    float Dot(const Vec3 &aVectorA, const Vec3 &aVectorB)
    {
        auto xx = aVectorA.x * aVectorB.x;
        auto yy = aVectorA.y * aVectorB.y;
        auto zz = aVectorA.z * aVectorB.z;

        return (xx + yy + zz);
    }

    /******************************************************************************/
    float Distance(const Vec3 &aVectorA, const Vec3 &aVectorB)
    {
        auto xVal = aVectorB.x - aVectorA.x;
        auto yVal = aVectorB.y - aVectorA.y;
        auto zVal = aVectorB.z - aVectorA.z;

        return std::sqrt(std::pow(xVal, 2) + std::pow(yVal, 2) + std::pow(zVal, 2));
    }
} // namespace KumaGL
