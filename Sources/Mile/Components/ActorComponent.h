#pragma once
#include "MileObject.h"

namespace Mile
{
    /**
    * ActorComponent�� Actor�� �߰��� �� �ִ� ���� ������ ������ �����ϴ� Component�� �⺻ Ŭ�����Դϴ�.
    */
    class Actor;
    class MILE_API ActorComponent : public Mile::Object
    {
        friend Actor;
    public:
        /**
        * ActorComponent�� �⺻ ������
        * @param NewName ������ ������Ʈ �̸�
        */
        ActorComponent( const MString& NewName ) :
            bIsTick( false ),
            OwnerPrivate( nullptr ),
            TickPriority( 0 ),
            Object( NewName )
        {
        }

        /**
        * ActorComponent�� �̵� ������
        * @param MovedObject �̵��� ������Ʈ
        */
        ActorComponent( ActorComponent&& MovedObject ) :
            OwnerPrivate( MovedObject.OwnerPrivate ),
            bIsTick( MovedObject.bIsTick ),
            TickPriority( MovedObject.TickPriority ),
            Object( std::move( MovedObject ) )
        {
            this->SetOwner( MovedObject.GetOwner( ) );
            MovedObject.SetOwner( nullptr );
            MovedObject.bIsTick = false;
            MovedObject.TickPriority = UINT64_MAX;
        }

        /**
        * ActorComponent�� �̵� ���� ������
        * @param MovedObject �̵��� ������Ʈ
        * @return �̵��� ������Ʈ
        */
        ActorComponent& operator=( ActorComponent&& MovedObject ) = delete;

        /**
        * ActorComponent�� �پ��ִ� ��ü(����)�� �����Ѵ�.
        * @param Owner ���� �������� ������ ����
        * @param bIsDetachBeforeSetOwner ���� �� ���� true�̸� ���ο� ������ �����ϱ� ���� ������ ���ο��Լ� �и��Ѵ�.
        */
        void SetOwner( Actor* Owner, bool bIsDetachBeforeSetNewOwner = true );

        /**
        * ȣ���� ActorComponent ��ü�� �� ��ü�� �ڽ� Component���� ���ε��� �ϰ������� �����Ѵ�.
        * @param Owner ���� �������� ������ ����
        * @param bIsDetachBeforeSetOwner ���� �� ���� true�̸� ���ο� ������ �����ϱ� ���� ������ ���ο��Լ� �и��Ѵ�.
        */
        virtual void SetOwnerRecursively( Actor* NewOwner, bool bIsDetachBeforeSetNewOwner = true );

        /**
        * ���� �پ��ִ� ������ ��ȯ�Ѵ�.
        * @return ���� �������� �����Ǿ��ִ� Actor
        */
        FORCEINLINE Actor* GetOwner( ) const
        {
            return OwnerPrivate;
        }

        /**
        * �� �����Ӹ��� ȣ��Ǵ� �޼ҵ�
        * @param DeltaTime �����Ӱ� �����ӻ��̿� �帥 �ð�
        */
        virtual void Tick( float DeltaTime ) { UNUSED_PARAM( DeltaTime ); }

    private:
        Actor* OwnerPrivate;
        bool    bIsTick;
        uint64  TickPriority;

    };
}