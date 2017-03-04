#include "SceneComponent.h"

namespace Mile
{
    bool SceneComponent::AttachTo( SceneComponent* NewParent )
    {
        bool bIsValidAttachment =
            NewParent != nullptr && ParentPrivate != NewParent && NewParent->GetParent( ) != this;
        if ( bIsValidAttachment )
        {
            DetachFromComponent( );
            ParentPrivate = NewParent;
            ParentPrivate->AddAttachedComponent( this );

            std::wcout << GetName( ).operator std::wstring( )
                << TEXT( " attach to " ) << ParentPrivate->GetName( ).operator std::wstring( ) << std::endl;

            auto ParentOwner = ParentPrivate->GetOwner( );
            auto Owner = this->GetOwner( );
            if ( ParentOwner != Owner )
            {
                SetOwnerRecursively( ParentOwner );
            }
            return true;
        }

        return false;
    }

    void SceneComponent::DetachFromComponent( )
    {
        bool bIsValidDetachment =
            ParentPrivate != nullptr;
        if ( bIsValidDetachment )
        {
            if ( ParentPrivate->RemoveAttachedComponent( this ) )
            {
                ParentPrivate = nullptr;
                SetOwnerRecursively( nullptr );
            }
        }
    }

    void SceneComponent::AddAttachedComponent( SceneComponent* Component )
    {
        Components.push_back( Component );
    }

    bool SceneComponent::RemoveAttachedComponent( SceneComponent* Component )
    {
        for ( auto FoundComponent = Components.begin( );
            FoundComponent != Components.end( );
            ++FoundComponent )
        {
            if ( ( *FoundComponent ) == Component )
            {
                Components.erase( FoundComponent );
                return true;
            }
        }

        return false;
    }

    void SceneComponent::SetOwnerRecursively( Actor* NewOwner, bool bIsDetachBeforeSetNewOwner )
    {
        this->SetOwner( NewOwner, bIsDetachBeforeSetNewOwner );
        for ( auto Child = Components.begin( );
            Child != Components.end( );
            ++Child )
        {
            ( *Child )->SetOwnerRecursively( NewOwner, bIsDetachBeforeSetNewOwner );
        }
    }

}