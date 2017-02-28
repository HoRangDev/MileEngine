#pragma once
#include "Mile.h"

namespace Mile
{
    class Object;
    /**
    * Base class of entire component class.
    */
    class MILE_API Component
    {
    public:
        Component( Object* Owner ) :
            bIsTick( false ),
            Owner( Owner )
        {
        }

        /* @todo: �̹� Owner�� ��ȿ�� ���¿��� ȣ��� �μ��ΰ� �۾� ���� */
        void SetOwner( Object* NewOwner );
        Object* GetOwner( ) const
        {
            return Owner;
        }

        virtual void Tick( float DeltaTime ) {}

    public:
        bool bIsTick;

    protected:
        Object* Owner;
        bool bIsRegistered;

    };
}