#pragma once
#include "Mile.h"
#include "MileString.h"

namespace Mile
{
    /*
    * @todo: �⺻ �ɷ� Ȯ�� ( ����ȭ, UUID, RTTI, �޸𸮰���<����غ���> )
    */
    class MILE_API Object
    {
    public:
        inline Object( const MString& NewName = MString( TEXT( "UNKNOWN" ) ), bool IsValid = true ) :
            Name( NewName ),
            bIsValid( IsValid ),
            ObjectID( NumOfAllocatedObject ),
            bIsTick( false ),
            bIsTickFuncRegistered( false ),
            TickPriority( 0 )
        {
            ++NumOfAllocatedObject;
            ++ObjectCounting;
        }

        Object( const Object& CopiedObject ) :
            Name( CopiedObject.Name ),
            bIsValid( CopiedObject.bIsValid ),
            ObjectID( NumOfAllocatedObject )
        {
            ++NumOfAllocatedObject;
            ++ObjectCounting;
        }

        Object( Object&& MovedObject ) :
            Name( std::move( MovedObject.Name ) ),
            bIsValid( MovedObject.bIsValid )
        {
        }

        virtual ~Object( )
        {
            --ObjectCounting;
        }

        void SetName( const MString& NewName )
        {
            Name = NewName;
        }

        MString GetName( ) const
        {
            return Name;
        }

        uint64 GetObjectID( ) const
        {
            return ObjectID;
        }

        static uint64 GetObjectCount( )
        {
            return ObjectCounting;
        }

        virtual void Tick( float DeltaTime ) { UNUSED_PARAM( DeltaTime ); }

        void SetIsTick( bool bNewIsTick );
        FORCEINLINE bool IsTick( ) const { return bIsTick; }

        void SetTickPriority( uint64 NewTickPriority );
        FORCEINLINE uint64 GetTickPriority( ) const { return TickPriority; }

    private:
        static uint64                NumOfAllocatedObject;
        static uint64                ObjectCounting;
        /* ObjectID �� �̶����� ������� Object�� ������ �Ҵ��. */
        uint64                       ObjectID;
        MString                      Name;
        bool                         bIsValid;

        bool                bIsTick;
        bool                bIsTickFuncRegistered;
        uint64              TickPriority;

    };
}