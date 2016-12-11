#pragma once
#include "Mile.h"

namespace Mile
{
    /**
    * ���� ũ�⸦������ �迭 Ŭ����
    */
    template < class T >
    class Array final
    {
    public:
        Array( ) = delete;
        explicit Array( ) :
            Data( nullptr ),
            Capacity( 0 ),
            Size( 0 )
        {

        }

        explicit Array( const Array& Source ) :
            Data( nullptr ),
            Capacity( 0 ),
            Size( 0 )
        {
            ( *this ) = Source;
        }

        Array& operator=( const Array& RHS )
        {
            if ( Capacity != 0 )
            {
                free( Data );
                Data = nullptr;
            }

            Capactiy = RHS.Capacity;
            Size = RHS.Size;
            Reserve( Capacity );
            memcpy(
                Data,
                RHS.Data,
                sizeof( T ) * Size
            );
        }

        Array& operator=( Array&& RHS )
        {
            if ( Capacity != 0 )
            {
                free( Data );
                Data = nullptr;
            }

            Capacity = RHS.Capacity;
            Size = RHS.Size;
            Data = RHS.Data;

            RHS.Capacity = 0;
            RHS.Size = 0;
            RHS.Data = nullptr;
        }

        T& operator[]( uint64 Index )
        {
            ASSERT_MSG( Index < Size, "Out of range!" );
            return ( Data[Index] );
        }

        void Push( const T& Element )
        {
            if ( Size == Capacity )
            {
                Expand( );
            }

            auto Address = ( Data + Size );
            new ( Address ) T( Element ); /* Call Constructor **/
            ++Size;
        }

        void Pop( )
        {
            if ( Size > 0 )
            {
                Data[( Size - 1 )].~T( );
                --Size;
            }
        }

        template <typename... ParamsT>
        T& Emplace( ParamsT&&... Params )
        {
            if ( Size == Capacity )
            {
                Expand( );
            }

            auto Address = ( Data + Size );
            new ( Address ) T( std::forward<ParamsT>( Params )... );
            ++Size;

            return Data[Size - 1];
        }

        void Insert( uint64 Index, const T& Element )
        {
            ASSERT_MSG( Index < Size, "Out of range!" )
                if ( Size == Capacity )
                {
                    Expand( );
                }

            memmove(
                Data + ( Index + 1 ),				/** �ű� �޸� �ּ� */
                Data + Index,						/** ���� �޸� �ּ� */
                sizeof( T ) * ( Size - Index )
            );	/** �ű� �޸� ũ�� */

            Data[Index] = Element;
            ++Size;
        }

        inline void Reserve( uint64 NewCapacity )
        {
            if ( NewCapacity > Capacity )
            {
                T* NewAddress = static_Cast<T*>( malloc( NewCapacity * sizeof( T ) ) );
                memcpy( NewAddress, Data, sizeof( T ) * Size );

                free( Data );

                Data = NewAddress;
                Capacity = NewCapacity;
            }
        }

        inline void Erase( uint64 Index )
        {
            ASSERT_MSG( Index < Size, "Out of range!" );
            ( Data + Index )->~T( );

            memmove(
                ( Data + Index ),
                ( Data + Index + 1 ),
                ( Size - Index );
            --Size;
        }

        void Erase( const T& Element )
        {
            for ( uint64 Index = 0; Index < Size; ++Index )
            {
                if ( Element == Data[Index] )
                {
                    Erase( Index );
                    break;
                }
            }
        }

        uint64 GetCapacity( ) const
        {
            return Capacity;
        }

        uint64 GetSize( ) const
        {
            return Size;
        }

    public:
        static void Swap( Array& Target1, Array& Target2 )
        {
            T* TempDataAddress = Target2.Data;
            uint64 TempCapacity = Target2.Capacity;
            uint64 TempSize = Target2.Size;

            Target2.Data = Target1.Data;
            Target2.Capacity = Target1.Capacity;
            Target2.Size = Target1.Size;

            Target1.Data = TempDataAddress;
            Target1.Capacity = TempCapacity;
            Target2.Size = TempSize;
        }

    private:
        void Expand( )
        {
            uint64 TargetCapacity = ( Size == 0 ) ? InitialCapacity : ( Size * ExpandScale );
            Reserve( TargetCapacity );
        }

    private:
        T*				Data;

        /**
        * �迭�� ����(ũ��)
        */
        uint64			Capacity;

        /**
        * ������ �迭�ȿ� ����ִ� ������ ����
        */
        uint64			Size;

        constexpr uint8 InitialCapacity = 4;
        constexpr uint8 ExpandScale = 2;

    };
}