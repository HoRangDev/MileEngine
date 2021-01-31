#pragma once
#include "Math/MathCore.h"

namespace Mile
{
   class MEAPI Vector2
   {
   public:
      Vector2( float xx, float yy ) :
         x( xx ),
         y( yy )
      {
      }

      Vector2( ) :
         Vector2( 0.0f, 0.0f )
      {
      }

      Vector2( const Vector2& vec ) :
         Vector2( vec.x, vec.y )
      {
      }

      Vector2 operator+( const Vector2& vec ) const 
      {
         return Vector2( x + vec.x,
                         y + vec.y );
      }

      Vector2 operator+( float val ) const
      {
         return Vector2( x + val,
                         y + val );
      }

      Vector2& operator+=( const Vector2& vec )
      {
         x += vec.x;
         y += vec.y;
         return ( *this );
      }

      Vector2& operator+=( float val )
      {
         x += val;
         y += val;
         return ( *this );
      }

      Vector2 operator-( const Vector2& vec ) const
      {
         return Vector2( x - vec.x,
                         y - vec.y );
      }

      Vector2 operator-( float val ) const
      {
         return Vector2( x - val,
                         y - val );
      }

      Vector2& operator-=( const Vector2& vec )
      {
         x -= vec.x;
         y -= vec.y;
         return ( *this );
      }

      Vector2& operator-=( float val )
      {
         x -= val;
         y -= val;
         return ( *this );
      }

      Vector2 operator*( const Vector2& vec ) const
      {
         return Vector2( x * vec.x,
                         y * vec.y );
      }

      Vector2 operator*( float factor ) const
      {
         return Vector2( x * factor,
                         y * factor );
      }

      Vector2& operator*=( const Vector2& vec )
      {
         x *= vec.x;
         y *= vec.y;
         return ( *this );
      }

      Vector2& operator*=( float factor )
      {
         x *= factor;
         y *= factor;
         return ( *this );
      }

      Vector2 operator/( const Vector2& vec ) const
      {
         return Vector2( x / vec.x,
                         y / vec.y );
      }

      Vector2 operator/( float div ) const
      {
         float factor = 1.0f / div;
         return Vector2( x * factor,
                         y * factor );
      }

      Vector2& operator/=( const Vector2& vec )
      {
         x /= vec.x;
         y /= vec.y;
         return ( *this );
      }

      Vector2& operator/=( float div )
      {
         float factor = 1.0f / div;
         x *= factor;
         y *= factor;
         return ( *this );
      }

      Vector2& operator=( const Vector2& vec )
      {
         x = vec.x;
         y = vec.y;
         return ( *this );
      }

	  json Serialize() const
	  {
		  json serialized;
		  serialized["x"] = x;
		  serialized["y"] = y;
		  return serialized;
	  }

      void DeSerialize( const json& jsonData )
      {
         x = GetValueSafelyFromJson(jsonData, "x", 0.0f);
         y = GetValueSafelyFromJson(jsonData, "y", 0.0f);
      }

      bool operator==( const Vector2& vec ) const
      {
         return ( x == vec.x ) && ( y == vec.y );
      }

      bool operator!=( const Vector2& vec ) const
      {
         return ( x != vec.x ) || ( y != vec.y );
      }

      float operator|( const Vector2& vec ) const
      {
         return Dot( vec );
      }

      float GetX( ) const
      {
         return x;
      }

      float GetY( ) const
      {
         return y;
      }

      void SetX( float xx )
      {
         x = xx;
      }

      void SetY( float yy )
      {
         y = yy;
      }

      float SizeSquared( ) const
      {
         return ( x * x + y * y );
      }

      float Size( ) const
      {
         return std::sqrt( SizeSquared( ) );
      }

      void Normalize( )
      {
         float factor = ( 1.0f / Size( ) );
         x *= factor;
         y *= factor;
      }

      Vector2 GetNormalized( ) const
      {
         Vector2 temp = ( *this );
         temp.Normalize( );
         return temp;
      }

      float Dot( const Vector2& vec ) const
      {
         return ( ( x*vec.x ) + ( y*vec.y ) );
      }

   public:
      union
      {
         struct
         {
            float x;
            float y;
         };

         float elements[ 2 ];
      };
   };
}