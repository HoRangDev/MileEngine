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
            ParentPrivate( nullptr ),
            ActorComponent( NewName )
        {
        }

        SceneComponent( SceneComponent&& MovedObject ) :
            ParentPrivate( nullptr ),
            ActorComponent( std::move( MovedObject ) )
        {
            AttachTo( MovedObject.GetParent( ) );
        }

        void SetTransform( const Transform& NewTransform )
        {
            Transform = NewTransform;
        }

        Transform& GetTransform( )
        {
            return Transform;
        }

        bool AttachTo( SceneComponent* NewParent );
        void DetachFromComponent( );

        virtual void SetOwnerRecursively( Actor* NewOwner, bool bIsDetachBeforeSetNewOwner = true ) override;

        SceneComponent* GetParent( ) const
        {
            return ParentPrivate;
        }

    protected:
        void AddAttachedComponent( SceneComponent* Component );
        bool RemoveAttachedComponent( SceneComponent* Component );

    private:
        Transform Transform;
        SceneComponent* ParentPrivate;
        ComponentList Children;

    };
}