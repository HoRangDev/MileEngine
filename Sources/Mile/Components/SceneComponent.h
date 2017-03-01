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
        SceneComponent( const MString& NewName ) :
            ActorComponent( NewName )
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

        FORCEINLINE ComponentList& GetAttachedComponents( ) { return Components; }

        bool AttachTo( SceneComponent* NewParent );
        void DetachFromComponent( );

        SceneComponent* GetParent( ) const
        {
            return ParentPrivate;
        }

    private:
        Transform Transform;
        SceneComponent* ParentPrivate;
        ComponentList Components;

    };
}