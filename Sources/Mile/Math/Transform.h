#pragma once
#include "Vector.h"

namespace Mile
{
    /**
    * ��ġ, ȸ��, ũ�� ������ �����ϰ� �̸� �̿��� �������� ������
    * �������ִ� ����ü�̴�.
    * @todo: Transform ������ �̿��� ��ȯ��� ���
    * @todo: �ٸ� Transform�� Merge �� �� �ֵ����ϱ�. ( T1 + T2 = T3 )
    * @todo: ���� ��ȯ����� ������ dirty ��ũ�� �������ʴٸ� ����� 
    *         ��ȯ����� ��ȯ. (���������� dirty ��ŷ ����)
    */
    struct MILE_API Transform
    {
    public:
        inline Transform( ) :
            bIsDirty( true ),
            Translation( Vector::Zero ),
            Rotation( Vector::Zero ),
            Scale( Vector::One )
        {
        }

        FORCEINLINE void SetTranslation( const Vector& Target )
        {
            bIsDirty = true;
            Translation = Target;
        }

        Vector GetTranslation( ) const
        {
            return Translation;
        }

        FORCEINLINE void SetRotation( const Vector& Target )
        {
            bIsDirty = true;
            Rotation = Target;
        }

        Vector GetRotation( ) const
        {
            return Rotation;
        }

        FORCEINLINE void SetScale( const Vector& Target )
        {
            bIsDirty = true;
            Scale = Target;
        }

        Vector GetScale( ) const
        {
            return Scale;
        }

        FORCEINLINE void SetIsDirty( bool bNewDirtyState = true )
        {
            bIsDirty = bNewDirtyState;
        }

        FORCEINLINE bool IsDirty( ) const
        {
            return bIsDirty;
        }


    private:
        bool   bIsDirty;
        Vector Translation;
        Vector Rotation;
        Vector Scale;
        //Matrix PastMatrix;

    };
}