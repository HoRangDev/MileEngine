#pragma once
#include "ActorComponent.h"
#include "Transform.h"
#include "Rumia/Array.h"

namespace Mile
{
    /**
    * SceneComponent�� ActorComponent�� Transform ������ Ȯ���ϰ� ���� ���ϼ��ִ� Component �̴�.
    */
    class MILE_API SceneComponent : public ActorComponent
    {
        using ComponentList = Rumia::Array<SceneComponent*>;
    protected:
        SceneComponent( Rumia::Allocator& Allocator, const MString& NewName ) :
            ParentPrivate( nullptr ),
            Children( Allocator ),
            ActorComponent( Allocator, NewName )
        {
        }

    public:
        void SetTransform( const Transform& NewTransform )
        {
            Transform = NewTransform;
        }

        Transform GetTransform( ETransformRelation Relation = ETransformRelation::Relative )
        {
            switch ( Relation )
            {
            default:
            case ETransformRelation::Absolute:
                return Transform;
                break;

            case ETransformRelation::Relative:
                if ( ParentPrivate != nullptr )
                {
                    return ( Transform + ParentPrivate->GetTransform( Relation ) );
                }
                break;
            }

            return Transform;
        }

        bool AttachTo( SceneComponent* NewParent );
        void DetachFromComponent( );

        virtual void SetOwnerRecursively( Actor* NewOwner, bool bIsDetachBeforeSetNewOwner = true ) override;

        SceneComponent* GetParent( ) const
        {
            return ParentPrivate;
        }

        Vector GetPosition( ETransformRelation Relation = ETransformRelation::Relative ) const
        {
            Vector AbsolutePosition = Transform.GetPosition( );
            switch ( Relation )
            {
            default:
            case ETransformRelation::Absolute:
                return AbsolutePosition;
                break;

            case ETransformRelation::Relative:
                if ( ParentPrivate != nullptr )
                {
                    return ( AbsolutePosition + ParentPrivate->GetPosition( Relation ) );
                }

                return AbsolutePosition;
                break;
            }
        }

        Vector GetRotation( ETransformRelation Relation = ETransformRelation::Relative ) const
        {
            Vector AbsoluteRotation = Transform.GetRotation( );
            switch ( Relation )
            {
            default:
            case ETransformRelation::Absolute:
                return AbsoluteRotation;
                break;

            case ETransformRelation::Relative:
                if ( ParentPrivate != nullptr )
                {
                    return ( AbsoluteRotation + ParentPrivate->GetRotation( Relation ) );
                }

                return AbsoluteRotation;
                break;
            }
        }

        Vector GetScale( ETransformRelation Relation = ETransformRelation::Relative ) const
        {
            Vector AbsoluteScale = Transform.GetScale( );
            switch ( Relation )
            {
            default:
            case ETransformRelation::Absolute:
                return AbsoluteScale;
                break;

            case ETransformRelation::Relative:
                if ( ParentPrivate != nullptr )
                {
                    return ( AbsoluteScale + ParentPrivate->GetScale( Relation ) );
                }

                return AbsoluteScale;
                break;
            }
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