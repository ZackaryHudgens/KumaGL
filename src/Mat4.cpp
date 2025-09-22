#include "KumaGL/Mat4.hpp"

#include <cmath>

#include "KumaGL/MathUtil.hpp"
#include "KumaGL/Vec3.hpp"

namespace KumaGL
{
    /******************************************************************************/
    Mat4::Mat4() { Identity(); }

    /******************************************************************************/
    Mat4::Mat4(float aData[4][4])
    {
        for (int r = 0; r < 4; ++r)
        {
            for (int c = 0; c < 4; ++c)
            {
                mData[c][r] = aData[c][r];
            }
        }
    }

    /******************************************************************************/
    Mat4::Mat4(float n00, float n10, float n20, float n30, float n01, float n11,
               float n21, float n31, float n02, float n12, float n22, float n32,
               float n03, float n13, float n23, float n33)
    {
        mData[0][0] = n00;
        mData[1][0] = n10;
        mData[2][0] = n20;
        mData[3][0] = n30;
        mData[0][1] = n01;
        mData[1][1] = n11;
        mData[2][1] = n21;
        mData[3][1] = n31;
        mData[0][2] = n02;
        mData[1][2] = n12;
        mData[2][2] = n22;
        mData[3][2] = n32;
        mData[0][3] = n03;
        mData[1][3] = n13;
        mData[2][3] = n23;
        mData[3][3] = n33;
    }

    /******************************************************************************/
    void Mat4::operator*=(const Mat4 &rhs) { (*this) = (*this) * rhs; }

    /******************************************************************************/
    void Mat4::Identity()
    {
        mData[0][0] = 1;
        mData[1][0] = 0;
        mData[2][0] = 0;
        mData[3][0] = 0;
        mData[0][1] = 0;
        mData[1][1] = 1;
        mData[2][1] = 0;
        mData[3][1] = 0;
        mData[0][2] = 0;
        mData[1][2] = 0;
        mData[2][2] = 1;
        mData[3][2] = 0;
        mData[0][3] = 0;
        mData[1][3] = 0;
        mData[2][3] = 0;
        mData[3][3] = 1;
    }

    /******************************************************************************/
    void Mat4::Scale(float x, float y, float z)
    {
        Mat4 mat(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
        (*this) *= mat;
    }

    /******************************************************************************/
    void Mat4::Scale(const KumaGL::Vec3 &aVec) { Scale(aVec.x, aVec.y, aVec.z); }

    /******************************************************************************/
    void Mat4::Translate(float x, float y, float z)
    {
        Mat4 mat(1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1);
        (*this) *= mat;
    }

    /******************************************************************************/
    void Mat4::Translate(const KumaGL::Vec3 &aVec)
    {
        Translate(aVec.x, aVec.y, aVec.z);
    }

    /******************************************************************************/
    void Mat4::Rotate(const KumaGL::Vec3 &aAxis, float aDegrees)
    {
        float c = std::cos(aDegrees * (PI / 180.0));
        float s = std::sin(aDegrees * (PI / 180.0));
        float d = 1.0 - c;

        float vx = aAxis.x;
        float vy = aAxis.y;
        float vz = aAxis.z;

        float vx2 = std::pow(vx, 2) * d;
        float vy2 = std::pow(vy, 2) * d;
        float vz2 = std::pow(vz, 2) * d;

        float vxvy = (vx * vy) * d;
        float vxvz = (vx * vz) * d;
        float vyvz = (vy * vz) * d;

        Mat4 mat(c + vx2, vxvy - s * vz, vxvz + s * vy, 0, vxvy + s * vz, c + vy2,
                 vyvz - s * vx, 0, vxvz - s * vy, vyvz + s * vx, c + vz2, 0, 0, 0, 0,
                 1);
        (*this) *= mat;
    }

    /******************************************************************************/
    void Mat4::Rotate(float x, float y, float z)
    {
        Rotate(KumaGL::Vec3(1, 0, 0), x);
        Rotate(KumaGL::Vec3(0, 1, 0), y);
        Rotate(KumaGL::Vec3(0, 0, 1), z);
    }

    /******************************************************************************/
    void Mat4::Rotate(const KumaGL::Vec3 &aVec) { Rotate(aVec.x, aVec.y, aVec.z); }

    /******************************************************************************/
    void Mat4::LookAt(const KumaGL::Vec3 &aPos, const KumaGL::Vec3 &aTarget,
                      const KumaGL::Vec3 &aUp)
    {
        // Create a new coordinate system using the camera's position and orientation.
        // First, calculate the direction vector from the target to the camera's
        // position (the z axis of the new coordinate system). Note that taking the
        // direction from the target to the camera (instead of from the camera to the
        // target) means that the z-axis will be positive behind the camera and
        // negative in front of it.
        auto zAxis = aTarget.Direction(aPos);

        // Calculate the vector perpendicular to the up and direction vectors (the x
        // axis of the new coordinate system).
        auto xAxis = Cross(Normalize(aUp), zAxis);
        xAxis.Normalize();

        // Calculate the camera's new up vector (the y coordinate of the new
        // coordinate system).
        auto yAxis = Cross(zAxis, xAxis);
        yAxis.Normalize();

        // Create a translation matrix for the new coordinate system.
        Mat4 translationMatrix;
        translationMatrix[3][0] = -aPos.x;
        translationMatrix[3][1] = -aPos.y;
        translationMatrix[3][2] = -aPos.z;

        // Create a rotation matrix for the new coordinate system.
        Mat4 rotationMatrix;
        rotationMatrix[0][0] = xAxis.x;
        rotationMatrix[1][0] = xAxis.y;
        rotationMatrix[2][0] = xAxis.z;
        rotationMatrix[0][1] = yAxis.x;
        rotationMatrix[1][1] = yAxis.y;
        rotationMatrix[2][1] = yAxis.z;
        rotationMatrix[0][2] = zAxis.x;
        rotationMatrix[1][2] = zAxis.y;
        rotationMatrix[2][2] = zAxis.z;

        // Finally, update this matrix to use the new coordinate system.
        (*this) = rotationMatrix * translationMatrix;
    }

    /******************************************************************************/
    void Mat4::Perspective(float aFOV, float aViewportWidth, float aViewportHeight,
                           float aNearPlane, float aFarPlane)
    {
        auto rad = aFOV * (PI / 180.0);
        auto f = 1.0 / std::tan(rad * 0.5);
        auto aspect = aViewportWidth / aViewportHeight;
        auto far = aFarPlane;
        auto near = aNearPlane;

        mData[0][0] = f / aspect;
        mData[1][0] = 0;
        mData[2][0] = 0;
        mData[3][0] = 0;
        mData[0][1] = 0;
        mData[1][1] = f;
        mData[2][1] = 0;
        mData[3][1] = 0;
        mData[0][2] = 0;
        mData[1][2] = 0;
        mData[2][2] = (near + far) / (near - far);
        mData[3][2] = (2 * far * near) / (near - far);
        mData[0][3] = 0;
        mData[1][3] = 0;
        mData[2][3] = -1;
        mData[3][3] = 0;
    }

    /******************************************************************************/
    void Mat4::Orthographic(float aLeft, float aRight, float aBottom, float aTop,
                            float aNear, float aFar)
    {
        auto rl = 2.0 / (aRight - aLeft);
        auto tb = 2.0 / (aTop - aBottom);
        auto fn = -2.0 / (aFar - aNear);
        auto rl2 = (aRight + aLeft) / (aRight - aLeft) * -1.0;
        auto tb2 = (aTop + aBottom) / (aTop - aBottom) * -1.0;
        auto fn2 = (aFar + aNear) / (aFar - aNear) * -1.0;

        mData[0][0] = rl;
        mData[1][0] = 0;
        mData[2][0] = 0;
        mData[3][0] = rl2;
        mData[0][1] = 0;
        mData[1][1] = tb;
        mData[2][1] = 0;
        mData[3][1] = tb2;
        mData[0][2] = 0;
        mData[1][2] = 0;
        mData[2][2] = fn;
        mData[3][2] = fn2;
        mData[0][3] = 0;
        mData[1][3] = 0;
        mData[2][3] = 0;
        mData[3][3] = 1;
    }

    /******************************************************************************/
    std::ostream &operator<<(std::ostream &os, const Mat4 &aMat)
    {
        for (int r = 0; r < 4; ++r)
        {
            for (int c = 0; c < 4; ++c)
            {
                os << aMat[c][r] << " ";
            }
            os << "\n";
        }

        return os;
    }

    /******************************************************************************/
    Mat4 operator*(const Mat4 &lhs, const Mat4 &rhs)
    {
        return Mat4(lhs[0][0] * rhs[0][0] + lhs[1][0] * rhs[0][1] +
                        lhs[2][0] * rhs[0][2] + lhs[3][0] * rhs[0][3],
                    lhs[0][0] * rhs[1][0] + lhs[1][0] * rhs[1][1] +
                        lhs[2][0] * rhs[1][2] + lhs[3][0] * rhs[1][3],
                    lhs[0][0] * rhs[2][0] + lhs[1][0] * rhs[2][1] +
                        lhs[2][0] * rhs[2][2] + lhs[3][0] * rhs[2][3],
                    lhs[0][0] * rhs[3][0] + lhs[1][0] * rhs[3][1] +
                        lhs[2][0] * rhs[3][2] + lhs[3][0] * rhs[3][3],
                    lhs[0][1] * rhs[0][0] + lhs[1][1] * rhs[0][1] +
                        lhs[2][1] * rhs[0][2] + lhs[3][1] * rhs[0][3],
                    lhs[0][1] * rhs[1][0] + lhs[1][1] * rhs[1][1] +
                        lhs[2][1] * rhs[1][2] + lhs[3][1] * rhs[1][3],
                    lhs[0][1] * rhs[2][0] + lhs[1][1] * rhs[2][1] +
                        lhs[2][1] * rhs[2][2] + lhs[3][1] * rhs[2][3],
                    lhs[0][1] * rhs[3][0] + lhs[1][1] * rhs[3][1] +
                        lhs[2][1] * rhs[3][2] + lhs[3][1] * rhs[3][3],
                    lhs[0][2] * rhs[0][0] + lhs[1][2] * rhs[0][1] +
                        lhs[2][2] * rhs[0][2] + lhs[3][2] * rhs[0][3],
                    lhs[0][2] * rhs[1][0] + lhs[1][2] * rhs[1][1] +
                        lhs[2][2] * rhs[1][2] + lhs[3][2] * rhs[1][3],
                    lhs[0][2] * rhs[2][0] + lhs[1][2] * rhs[2][1] +
                        lhs[2][2] * rhs[2][2] + lhs[3][2] * rhs[2][3],
                    lhs[0][2] * rhs[3][0] + lhs[1][2] * rhs[3][1] +
                        lhs[2][2] * rhs[3][2] + lhs[3][2] * rhs[3][3],
                    lhs[0][3] * rhs[0][0] + lhs[1][3] * rhs[0][1] +
                        lhs[2][3] * rhs[0][2] + lhs[3][3] * rhs[0][3],
                    lhs[0][3] * rhs[1][0] + lhs[1][3] * rhs[1][1] +
                        lhs[2][3] * rhs[1][2] + lhs[3][3] * rhs[1][3],
                    lhs[0][3] * rhs[2][0] + lhs[1][3] * rhs[2][1] +
                        lhs[2][3] * rhs[2][2] + lhs[3][3] * rhs[2][3],
                    lhs[0][3] * rhs[3][0] + lhs[1][3] * rhs[3][1] +
                        lhs[2][3] * rhs[3][2] + lhs[3][3] * rhs[3][3]);
    };

    /******************************************************************************/
    Vec3 operator*(const Mat4 &m, const Vec3 &v)
    {
        // This operation assumes a w-coordinate of 1.0.
        return Vec3(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * 1.0,
                    m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * 1.0,
                    m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * 1.0);
    };
} // namespace KumaGL
