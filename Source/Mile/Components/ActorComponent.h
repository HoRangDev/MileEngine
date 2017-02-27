#pragma once
#include "Mile.h"

namespace Mile
{
    class Actor;

    /**
    * ActorComponent�� Actor�� �߰��� �� �ִ� ���� ������ ������ �����ϴ� Component�� �⺻ Ŭ�����Դϴ�.
    */
    class MILE_API ActorComponent
    {
    public:
        ActorComponent( Actor* Owner ) :
            bIsTick( false ),
            Owner( Owner )
        {
        }

        Actor* GetOwner( ) const
        {
            return Owner;
        }

    public:
        bool bIsTick;

    private:
        Actor* Owner;
        bool bIsRegistered;

    };
}