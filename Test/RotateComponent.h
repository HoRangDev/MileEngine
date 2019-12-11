#pragma once

#include "CoreMinimal.h"

using namespace Mile;

class RotateComponent : public Component
{
   ComponentBegin( RotateComponent )

public:
   RotateComponent( Entity* entity ) :
      Component( entity )
   {
   }

   virtual void Update( )
   {
      auto timer = GetContext( )->GetSubSystem<Timer>( );
      auto transform = m_entity->GetTransform( );
      transform->Rotate( Quaternion( 180.0f * 0.2f * timer->GetDeltaTime( ), Vector3::Up( ) ) );
   }

   virtual json Serialize() const override
   {
	   json serialized = Component::Serialize();
	   serialized["Type"] = "RotateComponent";
	   return serialized;
   }

};
