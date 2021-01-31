#pragma once
#include <cmath>
#include "Core/CoreMinimal.h"

namespace Mile
{
   namespace Math
   {
      static const float Pi = 3.14159265f;

      static float DegreeToRadian(float degree)
      {
         return (degree / 180.0f) * Pi;
      }

      static float RadianToDegree(float radian)
      {
         return (radian / Pi) * 180.0f;
      }

      static float Lerp(float a, float b, float t)
      {
         return a + (t * (b - a));
      }

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
   }
}
