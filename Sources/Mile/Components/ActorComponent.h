#pragma once
#include "MileObject.h"

namespace Mile
{
    /**
    * ActorComponent�� Actor�� �߰��� �� �ִ� ���� ������ ������ �����ϴ� Component�� �⺻ Ŭ�����Դϴ�.
    */
    class Actor;
    class MILE_API ActorComponent : public MileObject
    {
        friend Actor;
    public:
        ActorComponent( const MString& NewName ) :
            bIsTick( false ),
            OwnerPrivate( nullptr ),
            TickPriority( 0 ),
            MileObject( NewName )
        {
        }

        ActorComponent( ActorComponent&& MovedObject ) :
            OwnerPrivate( MovedObject.OwnerPrivate ),
            bIsTick( MovedObject.bIsTick ),
            TickPriority( MovedObject.TickPriority ),
            MileObject( std::move( MovedObject ) )
        {
            this->SetOwner( MovedObject.GetOwner( ) );
            MovedObject.SetOwner( nullptr );
            MovedObject.bIsTick = false;
            MovedObject.TickPriority = UINT64_MAX;
        }

        ActorComponent& operator=( ActorComponent&& MovedObject ) = delete;

        void SetOwner( Actor* Owner, bool bIsDetachBeforeSetNewOwner = true );
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