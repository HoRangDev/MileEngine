#include "LightComponent.h"
#include "Core/Entity.h"
#include "Core/Transform.h"

namespace Mile
{
   LightComponent::LightComponent( Entity* entity ) :
      m_type( LightType::Directional ),
      m_color( Vector3( 1.0f, 1.0f, 1.0f ) ),
      m_spotlightAngles( Vector2( 0.0f, 0.0f ) ),
      m_lightRange( 100.0f ),
      Component( entity )
   {
   }

   Vector3 LightComponent::GetLightDirection( ) const
   {
      auto transform = m_entity->GetTransform( );
      return transform->GetForward( );
   }

   Vector3 LightComponent::GetLightPosition( ) const
   {
      auto transform = m_entity->GetTransform( );
      return transform->GetPosition( TransformSpace::World );
   }

   std::string LightComponent::Serialize( ) const
   {
      auto res = "{ " + Component::Serialize() + 
         ", \"Type\": " + LightTypeToString(m_type) + 
         ", \"Color\": " + m_color.Serialize( ) + 
         ", \"SpotlightAngles\": " + m_spotlightAngles.Serialize( ) +
         ", \"LightRange\": " + std::to_string( m_lightRange )
         + " }";

      return res;
   }

   void LightComponent::DeSerialize( const json& jsonData )
   {
      Component::DeSerialize( jsonData );
      m_type = StringToLightType( jsonData[ "Type" ] );
      m_color.DeSerialize( jsonData[ "Color" ] );
      m_spotlightAngles.DeSerialize( jsonData[ "SpotlightAngles" ] );
      m_lightRange = jsonData[ "LightRange" ];
   }
}