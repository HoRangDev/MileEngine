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
            OwnerPrivate( nullptr ),
            Object( NewName )
        {
        }

        /**
        * ActorComponent�� �̵� ������
        * @param MovedObject �̵��� ������Ʈ
        */
        ActorComponent( ActorComponent&& MovedObject ) :
            OwnerPrivate( MovedObject.OwnerPrivate ),
            Object( std::move( MovedObject ) )
        {
            this->SetOwner( MovedObject.GetOwner( ) );
            MovedObject.SetOwner( nullptr );
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

    private:
        Actor* OwnerPrivate;

    };
}