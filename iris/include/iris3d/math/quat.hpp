/*********************************************************
**  File name : quat.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
// IrisQuaternion Class
// (c) by Heinrich Tillack 2002 (http://a128.x15.org)
// under GPL or new BSD license
*********************************************************/

#ifndef __IRIS_QUATERNION_H__
#define __IRIS_QUATERNION_H__

#include "vect3d.hpp"

class IrisQuaternion
{
public:
  IrisQuaternion() {}

  IrisQuaternion(float sx, float sy, float sz, float sw)
      : x(sx),
        y(sy),
        z(sz),
        w(sw)
  {
  }
  ~IrisQuaternion(){};

  void inline Set(float sx, float sy, float sz, float sw)
  {
    x = sx, y = sy, z = sz, w = sw;
  }

  void Reset()
  {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 1.0f;
  }

  void CopyQuaternion(const IrisQuaternion &q)
  {
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
  }

  void CreateFromAxisAngle(float X, float Y, float Z, float degree)
  {
    // This function takes an angle and an axis of rotation, then converts
    // it to a IrisQuaternionernion.  An example of an axis and angle is what we pass into
    // glRotatef().  That is an axis angle rotation.  It is assumed an angle in
    // degrees is being passed in.  Instead of using glRotatef(), we can now handle
    // the rotations our self.

    // First we want to convert the degrees to radians
    // since the angle is assumed to be in radians
    float angle = IRIS_DEGTORAD(degree);

    // Here we calculate the sin( theta / 2) once for optimization
    float result = fsin(angle / 2.0f);

    // Calcualte the w value by cos( theta / 2 )
    w = fcos(angle / 2.0f);

    // Calculate the x, y and z of the IrisQuaternionernion
    x = X * result;
    y = Y * result;
    z = Z * result;
  }

  ////////////////////////////// CREATE MATRIX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
  /////
  /////	This function converts a IrisQuaternionernion to a rotation matrix
  /////
  ////////////////////////////// CREATE MATRIX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

  void CreateMatrix(float *pMatrix)
  {

    // First row
    pMatrix[0] = 1.0f - 2.0f * (y * y + z * z);
    pMatrix[1] = 2.0f * (x * y - w * z);
    pMatrix[2] = 2.0f * (x * z + w * y);
    pMatrix[3] = 0.0f;

    // Second row
    pMatrix[4] = 2.0f * (x * y + w * z);
    pMatrix[5] = 1.0f - 2.0f * (x * x + z * z);
    pMatrix[6] = 2.0f * (y * z - w * x);
    pMatrix[7] = 0.0f;

    // Third row
    pMatrix[8] = 2.0f * (x * z - w * y);
    pMatrix[9] = 2.0f * (y * z + w * x);
    pMatrix[10] = 1.0f - 2.0f * (x * x + y * y);
    pMatrix[11] = 0.0f;

    // Fourth row
    pMatrix[12] = 0;
    pMatrix[13] = 0;
    pMatrix[14] = 0;
    pMatrix[15] = 1.0f;

    // Now pMatrix[] is a 4x4 homogeneous matrix that can be applied to an OpenGL Matrix
  }

  void AxisAngleToQuaternion(const vect3d &axis, float theta)
  {
    float halfTheta = theta * 0.5f;
    float cosHalfTheta = fcos(halfTheta);
    float sinHalfTheta = fsin(halfTheta);
    x = axis.x * sinHalfTheta;
    y = axis.y * sinHalfTheta;
    z = axis.z * sinHalfTheta;
    w = cosHalfTheta;
  }

  void EulerToQuaternion(float roll, float pitch, float yaw)
  {
    float cr, cp, cy, sr, sp, sy, cpcy, spsy; // calculate trig identities
    cr = fcos(roll / 2);
    cp = fcos(pitch / 2);
    cy = fcos(yaw / 2);
    sr = fsin(roll / 2);
    sp = fsin(pitch / 2);
    sy = fsin(yaw / 2);
    cpcy = cp * cy;
    spsy = sp * sy;
    w = cr * cpcy + sr * spsy;
    x = sr * cpcy - cr * spsy;
    y = cr * sp * cy + sr * cp * sy;
    z = cr * cp * sy - sr * sp * cy;
  }

  float inline MagnitudeQuaternion()
  {
    // return mat_sqrtipr(x,y,z);
    return (fsqrt(w * w + x * x + y * y + z * z));
  }

  void NormaliseQuaternion()
  {
    float Mag;
    Mag = MagnitudeQuaternion();
    w = w / Mag;
    x = x / Mag;
    y = y / Mag;
    z = z / Mag;
  }

  void MultiplyQuaternion(const IrisQuaternion &q)
  {
    IrisQuaternion q3;
    vect3d vectorq1;
    vect3d vectorq2;
    vectorq1.x = x;
    vectorq1.y = y;
    vectorq1.z = z;
    vectorq2.x = q.x;
    vectorq2.y = q.y;
    vectorq2.z = q.z;
    vect3d tempvec1(vectorq1);
    vect3d tempvec2;
    vect3d tempvec3;
    tempvec1 = vectorq1;
    q3.w = (w * q.w) - tempvec1.Dot(vectorq2);
    tempvec1.Cross(vectorq2);
    tempvec2.x = w * q.x;
    tempvec2.y = w * q.y;
    tempvec2.z = w * q.z;
    tempvec3.x = q.w * x;
    tempvec3.y = q.w * y;
    tempvec3.z = q.w * z;
    x = tempvec1.x + tempvec2.x + tempvec3.x;
    y = tempvec1.y + tempvec2.y + tempvec3.y;
    z = tempvec1.z + tempvec2.z + tempvec3.z;
    w = q3.w;
    // CopyQuaternion(q3);
  }
  float x;
  float y;
  float z;
  float w;
};

#endif
