#pragma once
#include "ActorComponent.h"
#include "Transform.h"

namespace Mile
{
    /**
    * SceneComponent�� ActorComponent�� Transform ������ Ȯ���ϰ� ���� ���ϼ��ִ� Component �̴�.
    */
    class MILE_API SceneComponent : public ActorComponent
    {
        using ComponentList = std::vector<SceneComponent*>;
    public:
        SceneComponent( const MString& NewName, Actor* NewOwner ) :
            ActorComponent( NewName, NewOwner )
        {
        }

        void SetTransform( const Transform& NewTransform )
        {
            Transform = NewTransform;
        }

        Transform& GetTransform( )
        {
            return Transform;
        }

        void AttachComponent( SceneComponent* Target );
        bool DetachComponent( const SceneComponent* Target );

        void SetParent( SceneComponent* NewParent );
        SceneComponent* GetParent( ) const
        {
            return Parent;
        }

    private:
        void DetachFromParent( );

    private:
        Transform Transform;
        SceneComponent* Parent;
        ComponentList Components;

    };
}