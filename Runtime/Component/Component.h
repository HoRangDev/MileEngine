#pragma once

#include "../Core/Helper.h"

namespace Mile
{
   class Context;
   class MEAPI Component
   {
   public:
      Component( Context* context );
      virtual ~Component( ) { }

      template <typename Ty>
      static Ty* Create( Context* context )
      {
         return new Ty( context );
      }

      virtual std::string Serialize( ) const = 0;
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

   protected:
      Component( ) :
         Component( nullptr )
      {
      }

   protected:
      Context*        m_context;
      bool            m_bIsActive;

   };
}