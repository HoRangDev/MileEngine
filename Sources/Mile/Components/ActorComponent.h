#pragma once
#include "Component.h"

namespace Mile
{
    /**
    * ActorComponent�� Actor�� �߰��� �� �ִ� ���� ������ ������ �����ϴ� Component�� �⺻ Ŭ�����Դϴ�.
    */
    class MILE_API ActorComponent : public Component
    {
    public:
        ActorComponent( Object* Owner ) :
            Component( Owner )
        {
        }

    private:
        std::vector<ActorComponent*> Children;

    };
}