#ifndef MAT4_HPP
#define MAT4_HPP

#include <iostream>
#include <ostream>

#include "Vec3.hpp"

namespace KumaGL
{
    class Mat4
    {
    public:
        Mat4();
        Mat4(float aData[4][4]);
        Mat4(float n00, float n10, float n20, float n30, float n01, float n11,
             float n21, float n31, float n02, float n12, float n22, float n32,
             float n03, float n13, float n23, float n33);
        Mat4(Mat4 &&) = default;
        Mat4(const Mat4 &) = default;
        Mat4 &operator=(Mat4 &&) = default;
        Mat4 &operator=(const Mat4 &) = default;
        ~Mat4() = default;

        float *operator[](size_t index) { return mData[index]; }
        const float *operator[](size_t index) const { return mData[index]; }
        void operator*=(const Mat4 &rhs);

        const float *GetData() const { return mData[0]; }

        void Identity();

        void Scale(float x, float y, float z);
        void Scale(const KumaGL::Vec3 &aVec);
        void Translate(float x, float y, float z);
        void Translate(const KumaGL::Vec3 &aVec);
        void Rotate(const KumaGL::Vec3 &aAxis, float aDegrees);
        void Rotate(float x, float y, float z);
        void Rotate(const KumaGL::Vec3 &aVec);

        void LookAt(const KumaGL::Vec3 &aPos, const KumaGL::Vec3 &aTarget,
                    const KumaGL::Vec3 &aUp);
        void Perspective(float aFOV, float aViewportWidth, float aViewportHeight,
                         float aNearPlane, float aFarPlane);
        void Orthographic(float aLeft, float aRight, float aBottom, float aTop,
                          float aNear, float aFar);

        float mData[4][4];
    };

    std::ostream &operator<<(std::ostream &os, const Mat4 &aMat);
    Mat4 operator*(const Mat4 &lhs, const Mat4 &rhs);
    Vec3 operator*(const Mat4 &m, const Vec3 &v);
} // namespace KumaGL

#endif // !MAT4_HPP
