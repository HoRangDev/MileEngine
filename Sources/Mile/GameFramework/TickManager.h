#pragma once
#include "MileManager.hpp"
#include "Rumia/Array.h"

namespace Mile
{
    using TickFuncType = std::function<void( float )>;
    /* ���� ���ϼ��� �� ���� �켱������ ����. */
    struct MILE_API TickFunction
    {
        TickFuncType Func;
        uint64 ObjectID;
        uint64 Priority;
    };

    using ContainerType = Rumia::Array<TickFunction>;

    /**
    * Tick �̺�Ʈ�� �����ϴ� �Ŵ��� Ŭ����
    */
    class MILE_API TickManager final : public Mile::Manager<TickManager>
    {
        friend Manager;

    public:
        TickManager( const TickManager& ) = delete;
        TickManager& operator=( const TickManager& ) = delete;

        void AddEvent( TickFuncType Func, uint64 ObjectID, uint64 Priority = 0 );
        void RemoveEvent( uint64 ObjectID );
        void ModifyPriority( uint64 ObjectID, uint64 Priority );

        void Tick( float DeltaTime );

    private:
        TickManager( Rumia::Allocator& Allocator );
        ~TickManager( );

        void OrderingPriority( uint64 AddedPriority, bool ForceOrdering = false );
        bool IsAlreadyAdded( uint64 ObjectID ) const;

    private:
        ContainerType Container;
        uint64 RecentAddedPriority;

    };
}