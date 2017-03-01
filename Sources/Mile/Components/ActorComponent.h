#pragma once
#include "Object.h"

namespace Mile
{
    /**
    * ActorComponent�� Actor�� �߰��� �� �ִ� ���� ������ ������ �����ϴ� Component�� �⺻ Ŭ�����Դϴ�.
    */
    class Actor;
    class MILE_API ActorComponent : public Object
    {
        friend Actor;
    public:
        ActorComponent( const MString& NewName ) :
            bIsTick( false ),
            OwnerPrivate( nullptr ),
            Object( NewName )
        {
        }

        void SetOwner( Actor* Owner );
        FORCEINLINE Actor* GetOwner( ) const
        {
            return OwnerPrivate;
        }

        virtual void TickComponent( float DeltaTime ) {}

    private:
        Actor* OwnerPrivate;

    public:
        bool    bIsTick;
        uint64  TickPriority;

    };
}