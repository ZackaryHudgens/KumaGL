#ifndef VEC3_HPP
#define VEC3_HPP

#include <ostream>

namespace KumaGL
{
    class Vec3
    {
    public:
        Vec3() = default;
        Vec3(float a, float b, float c);
        Vec3(Vec3 &&) = default;
        Vec3(const Vec3 &) = default;
        Vec3 &operator=(Vec3 &&) = default;
        Vec3 &operator=(const Vec3 &) = default;
        ~Vec3() = default;

        void operator+=(const Vec3 &rhs);
        void operator-=(const Vec3 &rhs);
        void operator*=(float s);

        void Normalize();
        Vec3 Cross(const Vec3 &aVec) const;
        Vec3 Direction(const Vec3 &aVec) const;
        float Dot(const Vec3 &aVec) const;
        float Distance(const Vec3 &aVec) const;

        float x{0};
        float y{0};
        float z{0};
    };

    std::ostream &operator<<(std::ostream &os, const Vec3 &aVec);
    Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs);
    Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs);
    Vec3 operator*(const Vec3 &v, float s);

    Vec3 Normalize(const Vec3 &aVector);
    Vec3 Cross(const Vec3 &aVectorA, const Vec3 &aVectorB);
    Vec3 Direction(const Vec3 &aVectorA, const Vec3 &aVectorB);
    float Dot(const Vec3 &aVectorA, const Vec3 &aVectorB);
    float Distance(const Vec3 &aVectorA, const Vec3 &aVectorB);
} // namespace KumaGL

#endif // !VEC3_HPP
