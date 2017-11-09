#include "InputLayoutDX11.h"
#include "VertexShaderDX11.h"

namespace Mile
{
   bool InputLayoutDX11::Init( const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescs, VertexShaderDX11* shader )
   {
      if ( m_bIsInitialized || m_renderer == nullptr )
      {
         return false;
      }

      auto blob = shader->GetBlob( );
      auto result = m_renderer->GetDevice( )->CreateInputLayout( inputLayoutDescs.data( ),
                                                                 inputLayoutDescs.size( ),
                                                                 blob->GetBufferPointer( ),
                                                                 blob->GetBufferSize( ),
                                                                 &m_inputLayout );

      if ( FAILED( result ) )
      {
         return false;
      }

      m_bIsInitialized = true;
      return true;
   }

   bool InputLayoutDX11::Bind( )
   {
      if ( !m_bIsInitialized || m_renderer == nullptr )
      {
         return false;
      }

      m_renderer->GetDeviceContext( )->IASetInputLayout( m_inputLayout );

      return true;
   }
}