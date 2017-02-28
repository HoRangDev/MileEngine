#include "Actor.h"
#include "ActorComponent.h"
#include "SceneComponent.h"

namespace Mile
{
    void Actor::PostTick( float DeltaTime )
    {
        //@todo: ������Ʈ�� �߰��ǰų� �����Ǹ� �켱������� �� �����Ѵ�.
    }
    void Actor::TickActor( float DeltaTime )
    {
        for ( auto Component : Components )
        {
            if ( Component->bIsTick )
            {
                Component->TickComponent( DeltaTime );
            }
        }
    }

    void Actor::AttachComponent( ActorComponent* Target )
    {
        if ( Target != nullptr )
        {
            if ( Target->IsRegistered( ) )
            {
                /** �̹� Owner�� �ִ°�쿣 ���� Owner���� Detach ��Ų�� ������ Owner�� �����Ų��. */
                Target->SetOwner( this );
            }

            Target->OnAttachPost( );
        }
    }

    bool Actor::DetachComponent( const ActorComponent* Target )
    {
        if ( Target != nullptr )
        {
            for ( auto Itr = Components.begin( ); ( *Itr ) != Target; ++Itr )
            {
                ( *Itr )->OnDetachPost( );
                Components.erase( Itr );
                return true;
            }
        }
        return false;
    }

    void Actor::AttachRootComponent( SceneComponent* Target )
    {
        if ( Target != nullptr && RootComponent == nullptr )
        {
            AttachComponent( Target );
            RootComponent = Target;
        }
    }

    bool Actor::DetachRootComponent( )
    {
        if ( RootComponent != nullptr )
        {
            return DetachComponent( RootComponent );
        }

        return false;
    }

    SceneComponent* Actor::GetRootComponent( ) const
    {
        return RootComponent;
    }

}
