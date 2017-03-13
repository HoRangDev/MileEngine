#include "TickManager.h"
#include <algorithm>

namespace Mile
{
    TickManager* TickManager::Instance = nullptr;

    TickManager::TickManager( ) :
        RecentAddedPriority( 0 )
    {
    }

    TickManager::~TickManager( )
    {
        Container.clear( );
    }

    TickManager& TickManager::GetInstance( )
    {
        if ( Instance == nullptr )
        {
            Instance = new TickManager( );
        }

        return ( *Instance );
    }

    void TickManager::AddEvent( TickFuncType Func, uint64 ObjectID, uint64 Priority )
    {
        if ( !IsAlreadyAdded( ObjectID ) )
        {
            TickFunction FuncObject;
            FuncObject.Func = Func;
            FuncObject.ObjectID = ObjectID;
            FuncObject.Priority = Priority;
            Container.push_back( FuncObject );

            OrderingPriority( Priority );
        }
    }

    void TickManager::RemoveEvent( uint64 ObjectID )
    {
        for( auto TickObject = Container.begin( );
            TickObject != Container.end( );
            ++TickObject )
        {
            if ( ( *TickObject ).ObjectID == ObjectID )
            {
                Container.erase( TickObject );
                return;
            }
        }
    }

    bool TickManager::DestroyInstance( )
    {
        if ( Instance != nullptr )
        {
            delete Instance;
            Instance = nullptr;
            return true;
        }

        return false;
    }

    void TickManager::OrderingPriority( uint64 AddedPriority )
    {
        if ( RecentAddedPriority != AddedPriority )
        {
            RecentAddedPriority = AddedPriority;
            
            std::sort( Container.begin( ), Container.end( ),
                [ ]( TickFunction Left, TickFunction Right )->bool
            {
                return ( Left.Priority > Right.Priority );
            } );
        }
    }

    bool TickManager::IsAlreadyAdded( uint64 ObjectID ) const
    {
        for( const auto Object : Container )
        {
            if ( Object.ObjectID == ObjectID )
            {
                return true;
            }
        }

        return false;
    }
}