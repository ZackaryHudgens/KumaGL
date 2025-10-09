#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#include "Vec3.hpp"

namespace KumaGL
{
  const float PI = 3.141592;

  /******************************************************************************/
  inline float Lerp(float aStart, float aTarget, float aPercent)
  {
    return (aStart + (aTarget - aStart) * aPercent);
  }

  /******************************************************************************/
  inline Vec3 Lerp(const Vec3 &aStart, const Vec3 &aTarget, float aPercent)
  {
    return Vec3(Lerp(aStart.x, aTarget.x, aPercent),
                Lerp(aStart.y, aTarget.y, aPercent),
                Lerp(aStart.z, aTarget.z, aPercent));
  }
} // namespace KumaGL

#endif // !MATHUTIL_HPP
