#pragma once
#include "MileMath.h"

namespace Mile
{
    /**
     * 3���� �������� ��ġ �Ǵ� ������ ��Ÿ���� ���� Ŭ����.
    */
    struct MILE_API ALGIN_16 Vector3
    {
    public:
        /** ������ X ����. */
        float X;
        /** ������ Y ����. */
        float Y;
        /** ������ Z ����. */
        float Z;

    public:
        Vector3()
        {
        }

        /**
         * 3���� ������ ������
         *
         * @param InX X ��ǥ.
         * @param InY Y ��ǥ.
         * @param InZ Z ��ǥ.
        */
        explicit Vector3(float InX, float InY, float InZ)
            : X(InX), Y(InY), Z(InZ)
        {
        }

        /**
         * �ٸ� ������ ���е�� ���� ���������� �Ͽ� �� ����� ��ȯ��.
         *
         * @param Rhs ���� ����
        */
        Vector3 operator+(const Vector3& Rhs)
        {
            return Vector3(X + Rhs.X, Y + Rhs.Y, Z + Rhs.Z);
        }

        /**
         * �ٸ� ������ ���е�� ���� ���������� �Ͽ� �� ����� ��ȯ��.
         *
         * @param Rhs �� ����.
        */
        Vector3 operator-(const Vector3& Rhs)
        {
            return Vector3(X - Rhs.X, Y - Rhs.Y, Z - Rhs.Z);
        }

        /**
         * �ٸ� ������ ���е�� ���� ���������� �Ͽ� �� ����� ��ȯ��.
         *
         * @param Rhs ���� ����.
        */
        Vector3 operator*(const Vector3& Rhs)
        {
            return Vector3(X * Rhs.X, Y * Rhs.Y, Z * Rhs.Z);
        }

        /**
         * ������ ���е鿡 ���� �Ǽ��� ���Ѱ���� ��ȯ��.
         *
         * @param Rhs ���� �Ǽ�.
        */
        Vector3 operator*(float Rhs)
        {
            return Vector3(X * Rhs, Y * Rhs, Z * Rhs);
        }

        /**
         * �ٸ� ������ ���е�� ���� ������������ �Ͽ� �� ����� ��ȯ��.
         *
         * @param Rhs ���� ����.
        */
        Vector3 operator/(const Vector3& Rhs)
        {
            return Vector3(X / Rhs.X, Y / Rhs.Y, Z / Rhs.Z);
        }

        /**
         * ������ ���е鿡 ���� �Ǽ��� �������� ����� ��ȯ��.
         *
         * @param Rhs �������� �Ǽ�.
        */
        Vector3 operator/(float Rhs)
        {
            float InvRhs = 1.0f / Rhs;
            return Vector3(X * InvRhs, Y * InvRhs, Z * InvRhs);
        }

        /**
         * �ٸ� ������ ���е��� �ڽ��� ���е鿡�� ������.
         *
         * @param Rhs ���� ����.
        */
        Vector3 operator+=(const Vector3& Rhs)
        {
            X += Rhs.X;
            Y += Rhs.Y;
            Z += Rhs.Z;
            return *this;
        }

        /**
         * �ٸ� ������ ���е��� �ڽ��� ���е鿡�� ����.
         *
         * @param Rhs �� ����.
        */
        Vector3 operator-=(const Vector3& Rhs)
        {
            X -= Rhs.X;
            Y -= Rhs.Y;
            Z -= Rhs.Z;
            return *this;
        }

        /**
         * �ٸ� ������ ���е��� �ڽ��� ���е鿡 ������.
         *
         * @param Rhs ���� ����.
        */
        Vector3 operator*=(const Vector3& Rhs)
        {
            X *= Rhs.X;
            Y *= Rhs.Y;
            Z *= Rhs.Z;
            return *this;
        }

        /**
         * �Ǽ����� �ڽ��� ���п� ���� ������.
         *
         * @param Rhs ���� �Ǽ�.
        */
        Vector3 operator*=(float Rhs)
        {
            X *= Rhs;
            Y *= Rhs;
            Z *= Rhs;
            return *this;
        }

        /**
         * �ٸ� ������ ���е�� �ڽ��� ���е��� ��������.
         *
         * @param Rhs �������� ����.
        */
        Vector3 operator/=(const Vector3& Rhs)
        {
            X /= Rhs.X;
            Y /= Rhs.Y;
            Z /= Rhs.Z;
            return *this;
        }

        /**
         * �Ǽ������� �ڽ��� ���е��� ��������.
         *
         * @param Rhs �������� �Ǽ�.
        */
        Vector3 operator/=(float Rhs)
        {
            float InvRhs = 1.0f / Rhs;
            X *= InvRhs;
            Y *= InvRhs;
            Z *= InvRhs;
            return *this;
        }

        /**
         * �ڽ��� ����(Inverse Element)�� ����.
        */
        Vector3 operator-()
        {
            return Vector3(-X, -Y, -Z);
        }

        /**
         * ���е��� ������ ����.
         *
         * @param Rhs ���� 3���� ����.
        */
        bool operator==(const Vector3& Rhs)
        {
            return (X == Rhs.X) && (Y == Rhs.Y) && (Z == Rhs.Z);
        }

        /**
         * ���е��� �ٸ��� ����.
         *
         * @param Rhs ���� 3���� ����.
        */
        bool operator!=(const Vector3& Rhs)
        {
            return (X != Rhs.X) && (Y != Rhs.Y) && (Z != Rhs.Z);
        }

        /**
         * ���� ��������.
         *
         * @param Rhs ������������ 3���� ����.
        */
        float operator|(const Vector3& Rhs)
        {
            return this->DotProduct(Rhs);
        }

        /**
         * ���� ��������.
         *
         * @param Rhs ������������ 3���� ����(����).
        */
        Vector3 operator^(const Vector3& Rhs)
        {
            return this->CrossProduct(Rhs);
        }

        /**
         * ����ȭ�� ���͸� ��ȯ��. (�ڱ��ڽſ��� ������ ��ġ�� ����.)
        */
        Vector3 Normalized() const
        {
            float X = this->X;
            float Y = this->Y;
            float Z = this->Z;
            float InvLength = 1.0f / Math::Sqrt((X * X) + (Y * Y) + (Z * Z));
            return Vector3(X * InvLength, Y * InvLength, Z * InvLength);
        }

        /**
         * �ڽ��� ����ȭ��. (�ڱ��ڽſ��� ������ ��ħ.)
        */
        void Normalize()
        {
            float InvLength = 1.0f / Math::Sqrt((X * X) + (Y * Y) + (Z * Z));
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
        */
        void Set(float NewX, float NewY, float NewZ)
        {
            X = NewX;
            Y = NewY;
            Z = NewZ;
        }

        /**
         * X ���п� ���ο� ���� ������.
         *
         * @param NewX ���ο� X ��.
        */
        void SetX(float NewX)
        {
            X = NewX;
        }

        /**
         * Y ���п� ���ο� ���� ������.
         *
         * @param NewY ���ο� Y ��.
        */
        void SetY(float NewY)
        {
            Y = NewY;
        }

        /**
         * Z ���п� ���ο� ���� ������.
         *
         * @param NewZ ���ο� Z ��.
        */
        void SetZ(float NewZ)
        {
            Z = NewZ;
        }

        /**
         * X ������ ���� ��ȯ��.
        */
        float GetX() const
        {
            return X;
        }

        /**
         * Y ������ ���� ��ȯ��.
        */
        float GetY() const
        {
            return Y;
        }

        /**
         * Z ������ ���� ��ȯ��.
        */
        float GetZ() const
        {
            return Z;
        }

        /**
         * ������ ����(ũ��)�� ��ȯ��.
        */
        float Length() const
        {
            return Math::Sqrt((X * X) + (Y * Y) + (Z * Z));
        }

        /**
         * ������ ����(ũ��)�� ������ ��ȯ��.
        */
        float SquaredLength() const
        {
            return (X * X) + (Y * Y) + (Z * Z);
        }

        /**
         * ���� ������ ������� ��ȯ��.
         *
         * @param Rhs ���� �������� 3���� ����.
        */
        float DotProduct(const Vector3& Rhs)
        {
            return (X * Rhs.X) + (Y * Rhs.Y) + (Z * Rhs.Z);
        }

        /**
        * ���� ������ ������� ��ȯ��.
        *
        * @param Rhs ���� �������� 3���� ����.
        */
        Vector3 CrossProduct(const Vector3& Rhs)
        {
            float NewX = (Y * Rhs.Z) - (Z * Rhs.Z);
            float NewY = (Z * Rhs.X) - (X * Rhs.Z);
            float NewZ = (X * Rhs.Y) - (Y * Rhs.X);
            return Vector3(NewX, NewY, NewZ);
        }

        /**
         * �� ������ ���밪�� �������ΰ����� ���͸� ��ȯ��.
        */
        Vector3 GetAbs() const
        {
            return Vector3(Math::Abs(X), Math::Abs(Y), Math::Abs(Z));
        }

        /**
         * ��ҵ��� ��� 1���� Ȯ����.
        */
        bool IsOne() const
        {
            return (X == 1.0f) && (Y == 1.0f) && (Z == 1.0f);
        }

        /**
         * �� ����(Zero Vector)���� Ȯ����.
        */
        bool IsZero() const
        {
            return (X == 0.0f) && (Y == 0.0f) && (Z == 0.0f);
        }

        /**
         * ���� ����(Unit Vector)���� Ȯ����.
        */
        bool IsUnit() const
        {
            float Length = Math::Sqrt((X * X) + (Y * Y) + (Z * Z));
            return (Length == 1.0f);
        }
    };
}