#pragma once
#pragma once
#include "MileMath.h"

namespace Mile
{
	/**
	* ���� ��ǥ�踦 ��Ÿ���� ���� Ŭ����. (w = 1.0f => Point, w = 0.0f => Vector)
	*/
	struct MILE_API ALGIN_16 MVector4
	{
	public:
		float X;
		float Y;
		float Z;
		float W;

	public:
		MVector4( )
		{
		}

		/**
		* 4���� ������ ������
		*
		* @param InX X ��ǥ.
		* @param InY Y ��ǥ.
		* @param InZ Z ��ǥ.
		* @param InW W ��ǥ.
		*/
		explicit MVector4( float InX, float InY, float InZ, float InW = 1.0f )
			: X( InX ), Y( InY ), Z( InZ ), W( InW )
		{
		}

		/**
		* �ٸ� ������ ���е�� ���� ���������� �Ͽ� �� ����� ��ȯ��.
		*
		* @param Rhs ���� ����
		*/
		MVector4 operator+( const MVector4& Rhs )
		{
			return MVector4( X + Rhs.X, Y + Rhs.Y, Z + Rhs.Z, W + Rhs.W );
		}

		/**
		* �ٸ� ������ ���е�� ���� ���������� �Ͽ� �� ����� ��ȯ��.
		*
		* @param Rhs �� ����.
		*/
		MVector4 operator-( const MVector4& Rhs )
		{
			return MVector4( X - Rhs.X, Y - Rhs.Y, Z - Rhs.Z, W - Rhs.W );
		}

		/**
		* �ٸ� ������ ���е�� ���� ���������� �Ͽ� �� ����� ��ȯ��.
		*
		* @param Rhs ���� ����.
		*/
		MVector4 operator*( const MVector4& Rhs )
		{
			return MVector4( X * Rhs.X, Y * Rhs.Y, Z * Rhs.Z, W * Rhs.W );
		}

		/**
		* ������ ���е鿡 ���� �Ǽ��� ���Ѱ���� ��ȯ��.
		*
		* @param Rhs ���� �Ǽ�.
		*/
		MVector4 operator*( float Rhs )
		{
			return MVector4( X * Rhs, Y * Rhs, Z * Rhs, W * Rhs );
		}

		/**
		* �ٸ� ������ ���е�� ���� ������������ �Ͽ� �� ����� ��ȯ��.
		*
		* @param Rhs ���� ����.
		*/
		MVector4 operator/( const MVector4& Rhs )
		{
			return MVector4( X / Rhs.X, Y / Rhs.Y, Z / Rhs.Z, W / Rhs.W );
		}

		/**
		* ������ ���е鿡 ���� �Ǽ��� �������� ����� ��ȯ��.
		*
		* @param Rhs �������� �Ǽ�.
		*/
		MVector4 operator/( float Rhs )
		{
			float InvRhs = 1.0f / Rhs;
			return MVector4( X * InvRhs, Y * InvRhs, Z * InvRhs, W * InvRhs );
		}

		/**
		* �ٸ� ������ ���е��� �ڽ��� ���е鿡�� ������.
		*
		* @param Rhs ���� ����.
		*/
		MVector4 operator+=( const MVector4& Rhs )
		{
			X += Rhs.X;
			Y += Rhs.Y;
			Z += Rhs.Z;
			W += Rhs.W;
			return *this;
		}

		/**
		* �ٸ� ������ ���е��� �ڽ��� ���е鿡�� ����.
		*
		* @param Rhs �� ����.
		*/
		MVector4 operator-=( const MVector4& Rhs )
		{
			X -= Rhs.X;
			Y -= Rhs.Y;
			Z -= Rhs.Z;
			W -= Rhs.W;
			return *this;
		}

		/**
		* �ٸ� ������ ���е��� �ڽ��� ���е鿡 ������.
		*
		* @param Rhs ���� ����.
		*/
		MVector4 operator*=( const MVector4& Rhs )
		{
			X *= Rhs.X;
			Y *= Rhs.Y;
			Z *= Rhs.Z;
			W *= Rhs.W;
			return *this;
		}

		/**
		* �Ǽ����� �ڽ��� ���п� ���� ������.
		*
		* @param Rhs ���� �Ǽ�.
		*/
		MVector4 operator*=( float Rhs )
		{
			X *= Rhs;
			Y *= Rhs;
			Z *= Rhs;
			W *= Rhs;
			return *this;
		}

		/**
		* �ٸ� ������ ���е�� �ڽ��� ���е��� ��������.
		*
		* @param Rhs �������� ����.
		*/
		MVector4 operator/=( const MVector4& Rhs )
		{
			X /= Rhs.X;
			Y /= Rhs.Y;
			Z /= Rhs.Z;
			W /= Rhs.W;
			return *this;
		}

		/**
		* �Ǽ������� �ڽ��� ���е��� ��������.
		*
		* @param Rhs �������� �Ǽ�.
		*/
		MVector4 operator/=( float Rhs )
		{
			float InvRhs = 1.0f / Rhs;
			X *= InvRhs;
			Y *= InvRhs;
			Z *= InvRhs;
			W *= InvRhs;
			return *this;
		}

		/**
		* �ڽ��� ����(Inverse Element)�� ����.
		*/
		MVector4 operator-( )
		{
			return MVector4( -X, -Y, -Z, -W );
		}

		/**
		* ���е��� ������ ����.
		*
		* @param Rhs ���� 3���� ����.
		*/
		bool operator==( const MVector4& Rhs )
		{
			return ( X == Rhs.X ) && ( Y == Rhs.Y ) && ( Z == Rhs.Z ) && ( W == Rhs.W );
		}

		/**
		* ���е��� �ٸ��� ����.
		*
		* @param Rhs ���� 3���� ����.
		*/
		bool operator!=( const MVector4& Rhs )
		{
			return ( X != Rhs.X ) && ( Y != Rhs.Y ) && ( Z != Rhs.Z ) && ( W != Rhs.W );
		}

		/**
		* 3���� ���� ��ҿ� ���� ���� ��������.
		*
		* @param Rhs ������������ 4���� ����.
		*/
		float operator|( const MVector4& Rhs )
		{
			return this->DotProduct3( Rhs );
		}

		/**
		* 3���� ���� ��ҿ� ���� ���� ��������.
		*
		* @param Rhs ������������ 4���� ����(����).
		*/
		MVector4 operator^( const MVector4& Rhs )
		{
			return this->CrossProduct3( Rhs );
		}

		/**
		* 3���� ���� ��ҿ� ���� ����ȭ�� ���͸� ��ȯ��. (�ڱ��ڽſ��� ������ ��ġ�� ����.)
		*/
		MVector4 Normalized3( ) const
		{
			float X = this->X;
			float Y = this->Y;
			float Z = this->Z;
			float InvLength = 1.0f / Math::Sqrt( ( X * X ) + ( Y * Y ) + ( Z * Z ) );
			return MVector4( X * InvLength, Y * InvLength, Z * InvLength, W );
		}

		/**
		* 3���� ���� ��ҿ� ���� �ڽ��� ����ȭ��. (�ڱ��ڽſ��� ������ ��ħ.)
		*/
		void Normalize3( )
		{
			float InvLength = 1.0f / Math::Sqrt( ( X * X ) + ( Y * Y ) + ( Z * Z ) );
			X *= InvLength;
			Y *= InvLength;
			Z *= InvLength;
		}

		/**
		* ���е鿡 ���ο� ���� ������.
		*
		* @param NewX ���ο� X ��.
		* @param NewY ���ο� Y ��.
		* @param NewZ ���ο� Z ��.
		* @param NewW ���ο� W ��.
		*/
		void Set( float NewX, float NewY, float NewZ, float NewW )
		{
			X = NewX;
			Y = NewY;
			Z = NewZ;
			W = NewW;
		}

		/**
		* X ���п� ���ο� ���� ������.
		*
		* @param NewX ���ο� X ��.
		*/
		void SetX( float NewX )
		{
			X = NewX;
		}

		/**
		* Y ���п� ���ο� ���� ������.
		*
		* @param NewY ���ο� Y ��.
		*/
		void SetY( float NewY )
		{
			Y = NewY;
		}

		/**
		* Z ���п� ���ο� ���� ������.
		*
		* @param NewZ ���ο� Z ��.
		*/
		void SetZ( float NewZ )
		{
			Z = NewZ;
		}

		/**
		* W ���п� ���ο� ���� ������.
		*
		* @param NewW ���ο� W ��.
		*/
		void SetW( float NewW )
		{
			W = NewW;
		}

		/**
		* X ������ ���� ��ȯ��.
		*/
		float GetX( ) const
		{
			return X;
		}

		/**
		* Y ������ ���� ��ȯ��.
		*/
		float GetY( ) const
		{
			return Y;
		}

		/**
		* Z ������ ���� ��ȯ��.
		*/
		float GetZ( ) const
		{
			return Z;
		}

		/**
		* W ������ ���� ��ȯ��.
		*/
		float GetW( ) const
		{
			return W;
		}

		/**
		* ������ ����(ũ��)�� ��ȯ��.
		*/
		float Length( ) const
		{
			return Math::Sqrt( ( X * X ) + ( Y * Y ) + ( Z * Z ) + ( W * W ) );
		}

		/**
		* 3���� ���� ��ҿ� ���� ������ ����(ũ��)�� ��ȯ��.
		*/
		float Length3( ) const
		{
			return Math::Sqrt( ( X * X ) + ( Y * Y ) + ( Z * Z ) );
		}

		/**
		* ������ ����(ũ��)�� ������ ��ȯ��.
		*/
		float SquaredLength( ) const
		{
			return ( X * X ) + ( Y * Y ) + ( Z * Z ) + ( W * W );
		}

		/**
		* 3���� ���� ��ҿ� ���� ������ ����(ũ��)�� ������ ��ȯ��.
		*/
		float SquaredLength3( ) const
		{
			return ( X * X ) + ( Y * Y ) + ( Z * Z );
		}

		/**
		* 3���� ���� ��ҿ� ���� ���� ������ ������� ��ȯ��.
		*
		* @param Rhs ���� �������� 4���� ����.
		*/
		float DotProduct3( const MVector4& Rhs )
		{
			return ( X * Rhs.X ) + ( Y * Rhs.Y ) + ( Z * Rhs.Z );
		}

		/**
		* 3���� ���� ��ҿ� ���� ���� ������ ������� ��ȯ��.
		*
		* @param Rhs ���� �������� 4���� ����.
		*/
		MVector4 CrossProduct3( const MVector4& Rhs )
		{
			float NewX = ( Y * Rhs.Z ) - ( Z * Rhs.Z );
			float NewY = ( Z * Rhs.X ) - ( X * Rhs.Z );
			float NewZ = ( X * Rhs.Y ) - ( Y * Rhs.X );
			return MVector4( NewX, NewY, NewZ, 0.0f );
		}

		/**
		* �� ������ ���밪�� �������ΰ����� ���͸� ��ȯ��.
		*/
		MVector4 GetAbs( ) const
		{
			return MVector4( Math::Abs( X ), Math::Abs( Y ), Math::Abs( Z ), Math::Abs( W ) );
		}

		/**
		* ��ҵ��� ��� 1���� Ȯ����.
		*/
		bool IsOne( ) const
		{
			return ( X == 1.0f ) && ( Y == 1.0f ) && ( Z == 1.0f ) && ( W == 1.0f );
		}

		/**
		* �� ����(Zero Vector)���� Ȯ����.
		*/
		bool IsZero( ) const
		{
			return ( X == 0.0f ) && ( Y == 0.0f ) && ( Z == 0.0f ) && ( W == 0.0f );
		}

		/**
		* ���� ����(Unit Vector)���� Ȯ����.
		*/
		bool IsUnit( ) const
		{
			float Length = Math::Sqrt( ( X * X ) + ( Y * Y ) + ( Z * Z ) + ( W * W ) );
			return ( Length == 1.0f );
		}

		/**
		* 3���� ���� ��ҿ� ���� ���� ����(Unit Vector)���� Ȯ����.
		*/
		bool IsUnit3( ) const
		{
			float Length = Math::Sqrt( ( X * X ) + ( Y * Y ) + ( Z * Z ) );
			return ( Length == 1.0f );
		}
	};
}