#pragma once
#include "MileObject.h"


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

    using ContainerType = std::vector<TickFunction>;

    /**
    * Tick �̺�Ʈ�� �����ϴ� �Ŵ��� Ŭ����
    */
    class MILE_API TickManager final : public Mile::Object
    {
    public:
        TickManager( const TickManager& ) = delete;
        ~TickManager( );

        TickManager& operator=( const TickManager& ) = delete;

        static TickManager& GetInstance( );
        static bool DestroyInstance( );

        void AddEvent( TickFuncType Func, uint64 ObjectID, uint64 Priority = 0 );
        void RemoveEvent( uint64 ObjectID );
        void ModifyPriority( uint64 ObjectID, uint64 Priority );

        void Tick( float DeltaTime );

    private:
        TickManager( );

        void OrderingPriority( uint64 AddedPriority, bool ForceOrdering = false );
        bool IsAlreadyAdded( uint64 ObjectID ) const;

    private:
        static TickManager* Instance;
        ContainerType Container;
        uint64 RecentAddedPriority;

    };
}