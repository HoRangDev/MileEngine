#pragma once

#include "Core/Helper.h"

namespace Mile
{
   class Entity;
   class Transform;
   class MEAPI Component
   {
   public:
      Component( Entity* entity );
      virtual ~Component( ) { }

      template <typename Ty>
      static Ty* Create( Entity* entity )
      {
         return new Ty( entity );
      }

      virtual std::string Serialize( ) const { return ( "\"IsActivated\": " + Mile::BoolSerialize( m_bIsActive )); }
      virtual void DeSerialize( const json& jsonData )
      {
         m_bIsActive = jsonData[ "IsActivated" ];
      }

      bool IsActive( ) const { return m_bIsActive; }
      void SetActive( bool bIsActive );

      virtual void Reset( ) { }
      virtual void Start( ) { }
      virtual void Update( ) { }
      virtual void OnEnable( ) { }
      virtual void OnDisable( ) { }

      Entity* GetEntity( ) const { return m_entity; }
      Transform* GetTransform( ) const;

   protected:
      Component( ) :
         Component( nullptr )
      {
      }

   protected:
      Entity*         m_entity;
      bool            m_bIsActive;

   };
}