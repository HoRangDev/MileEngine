#pragma once
#include "ActorComponent.h"

namespace Mile
{
    /**
    * SceneComponent�� Transform ������ ������ ���� ���̴� ���� �����ϴ� ������Ʈ�Դϴ�.
    */
    class MILE_API SceneComponent : public ActorComponent
    {
    public:
        SceneComponent( Actor* Owner ) :
            ActorComponent( Owner )
        {
        }

    private:
        std::vector<SceneComponent*> Children;

    };
}