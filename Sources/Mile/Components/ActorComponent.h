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
        ActorComponent( const MString& NewName, Actor* NewOwner ) :
            Owner( NewOwner ),
            bIsRegistered( true ),
            bIsTick( false ),
            Object( NewName )
        {
        }

        void SetOwner( Actor* NewOwner );
        Actor* GetOwner( ) const
        {
            return Owner;
        }

        virtual void TickComponent( float DeltaTime ) {}

        FORCEINLINE bool IsRegistered( ) const
        {
            return bIsRegistered;
        }

    protected:
        virtual void OnAttachPost( )
        {
            bIsRegistered = true;
            OnAttach( );
        }

        virtual void OnDetachPost( )
        {
            bIsRegistered = false;
            Owner = nullptr;
            OnDetach( );
        }

        virtual void OnAttach( ) {}
        virtual void OnDetach( ) {}

        void DetachFromOwner( );

    private:
        bool bIsRegistered;
        Actor* Owner;

    public:
        bool    bIsTick;
        uint64  TickPriority;

    };
}