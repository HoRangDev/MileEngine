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
            ObjectID( NumOfAllocatedObject ),
            bIsTick( CopiedObject.bIsTick ),
            bIsTickFuncRegistered( false ),
            TickPriority( CopiedObject.TickPriority )
        {
            SetIsTick( bIsTick );
            ++NumOfAllocatedObject;
            ++ObjectCounting;
        }

        Object( Object&& MovedObject ) :
            Name( std::move( MovedObject.Name ) ),
            bIsValid( MovedObject.bIsValid ),
            bIsTick( MovedObject.bIsTick ),
            bIsTickFuncRegistered( MovedObject.bIsTickFuncRegistered ),
            TickPriority( MovedObject.TickPriority )
        {
            MovedObject.SetIsTick( false );
            this->SetIsTick( bIsTick );
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

        /**
        * �� �����Ӹ��� ȣ��Ǵ� �޼ҵ�
        * @param DeltaTime �����Ӱ� �����ӻ��̿� �帥 �ð�
        */
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

        bool                         bIsTick;
        bool                         bIsTickFuncRegistered;
        uint64                       TickPriority;

    };
}