#include "Context.h"
#include "ComponentRegister.h"

namespace Mile
{
   Context::Context( )
   {
   }

   Context::~Context( )
   {
      for ( size_t idx = 1; idx < m_subSystems.size( ) - 1; ++idx )
      {
         SafeDelete( m_subSystems[ idx ] );
      }

      ComponentRegister::Destroy( );
   }

   void Context::RegisterSubSystem( SubSystem* newSubSystem )
   {
      if ( newSubSystem != nullptr )
      {
         m_subSystems.push_back( newSubSystem );
      }
   }
}