#pragma once
#include "MathCore.h"
#include "Math/Quaternion.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace Mile
{
   namespace Math
   {
      static float ClampAngle(float degree)
      {
         degree = std::fmod(degree, 360.0f);
         if (degree < 0.0f)
         {
            degree += 360.0f;
         }

         return degree;
      }

      static float NormalizeAngle(float degree)
      {
         degree = ClampAngle(degree);
         if (degree > 180.0f)
         {
            degree -= 360.0f;
         }

         return degree;
      }

      static Vector3 RadEulerAnglesToDegEulerAngles(const Vector3& radEuler)
      {
         return Vector3(
            RadianToDegree(radEuler.x),
            RadianToDegree(radEuler.y),
            RadianToDegree(radEuler.z));
      }

      static Vector3 DegEulerAnglesToRadEulerAngles(const Vector3& degEuler)
      {
         return Vector3(
            DegreeToRadian(degEuler.x),
            DegreeToRadian(degEuler.y),
            DegreeToRadian(degEuler.z));
      }

      /* Degree Euler Angles to Quaternion */
      // +angles = cw(clock wise) rotation for (Roll, Yaw)
      static Quaternion EulerToQuaternion(const Vector3& eulerAngles)
      {
         // Pitch = x, Yaw = y, Roll = z
         Vector3 target = DegEulerAnglesToRadEulerAngles(eulerAngles);
         target.x = std::fmod(target.x, 360.0f);
         target.y = std::fmod(target.y, 360.0f);
         target.z = std::fmod(target.z, 360.0f);
         float cy = cos(target.z * 0.5f);
         float sy = sin(target.z * 0.5f);
         float cp = cos(target.y * 0.5f);
         float sp = sin(target.y * 0.5f);
         float cr = cos(target.x * 0.5f);
         float sr = sin(target.x * 0.5f);

         Quaternion q;
         q.w = cr * cp * cy + sr * sp * sy;
         q.x = sr * cp * cy - cr * sp * sy;
         q.y = cr * sp * cy + sr * cp * sy;
         q.z = cr * cp * sy - sr * sp * cy;
         q.Normalize();

         return q;
      }

      /* Quaternion To Degree Euler Angles */
      static Vector3 QuaternionToEulerAngles(const Quaternion& target)
      {
         constexpr float SigularityThreshold = 0.4999999f;
         Quaternion q = target.Normalized();
         Vector3 angles;

         float test = q.x * q.y + q.z * q.w;
         if (test > SigularityThreshold)
         {
            angles.y = 2.0f * atan2(q.x, q.w);
            angles.z = Pi * 0.5f;
            angles.x = 0.0f;
         }
         else if (test < -SigularityThreshold)
         {
            angles.y = -2.0f * atan2(q.x, q.w);
            angles.z = -Pi * 0.5f;
            angles.x = 0.0f;
         }
         else
         {
            float sqx = q.x * q.x;
            float sqy = q.y * q.y;
            float sqz = q.z * q.z;
            angles.y = atan2(2.0f*(q.y * q.w -q.x * q.z), 1.0f - 2.0f * (sqy - sqz));
            angles.z = asin(2.0f * test);
            angles.x = atan2(2.0f * (q.x * q.w - q.y * q.z), 1.0f - 2.0f * (sqx - sqz));
         }

         //angles.y = -angles.y;
         //angles.z = -angles.z;
         return RadEulerAnglesToDegEulerAngles(angles);
      }
   }
}
