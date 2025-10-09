#include "MathTests.hpp"

#include <iostream>

int main()
{
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Vec3 addition..." << std::endl;
  KumaGL::TestVec3Addition();
  std::cout << "Vec3 addition successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Vec3 subtraction..." << std::endl;
  KumaGL::TestVec3Subtraction();
  std::cout << "Vec3 subtraction successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Mat4 multiplication..." << std::endl;
  KumaGL::TestMat4Multiplication();
  std::cout << "Mat4 multiplication successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Mat4 multiplication with Vec3..." << std::endl;
  KumaGL::TestMat4MultiplicationWithVec3();
  std::cout << "Mat4 multiplication with Vec3 successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Mat4 translation..." << std::endl;
  KumaGL::TestMat4Translation();
  std::cout << "Mat4 translation successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Mat4 rotation..." << std::endl;
  KumaGL::TestMat4Rotation();
  std::cout << "Mat4 rotation successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Mat4 scaling..." << std::endl;
  KumaGL::TestMat4Scaling();
  std::cout << "Mat4 scaling successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing linear interpolation..." << std::endl;
  KumaGL::TestLerp();
  std::cout << "Linear interpolation successful!" << std::endl;

  return 0;
}
