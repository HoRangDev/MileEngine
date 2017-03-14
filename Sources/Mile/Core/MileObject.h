#pragma once
#include "Mile.h"
#include "MileString.h"

namespace Mile
{
    /*
    * @todo: �⺻ �ɷ� Ȯ�� ( ����ȭ, UUID, RTTI, �޸𸮰���<����غ���> ) 
    */
    class MILE_API MileObject
    {
    public:
        inline MileObject( const MString& NewName = MString( TEXT("UNKNOWN") ), bool IsValid = true ) :
            Name( NewName ),
            bIsValid( IsValid ),
            ObjectID( NumOfAllocatedObject )
        {
            ++NumOfAllocatedObject;
            ++ObjectCounting;
        }

        MileObject( const MileObject& CopiedObject ) :
            Name( CopiedObject.Name ),
            bIsValid( CopiedObject.bIsValid ),
            ObjectID( NumOfAllocatedObject )
        {
            ++NumOfAllocatedObject;
            ++ObjectCounting;
        }

        MileObject( MileObject&& MovedObject ) :
            Name( std::move( MovedObject.Name ) ),
            bIsValid( MovedObject.bIsValid )
        {
        }

        virtual ~MileObject( )
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

        static uint64 GetObjectCount()
        {
            return ObjectCounting;
        }

    private:
        static uint64                NumOfAllocatedObject;
        static uint64                ObjectCounting;
        /* ObjectID �� �̶����� ������� Object�� ������ �Ҵ��. */
        uint64                       ObjectID;
        MString                      Name;
        bool                         bIsValid;

    };
}