#include "Viewport.h"
#include "RendererDX11.h"

namespace Mile
{
   Viewport::Viewport( RendererDX11* renderer ) :
      m_renderer( renderer ),
      m_width( 800.0f ), m_height( 600.0f ),
      m_minDepth( 0.0f ), m_maxDepth( 1.0f ),
      m_topLeftX( 0.0f ), m_topLeftY( 0.0f )
   {
   }

   D3D11_VIEWPORT Viewport::GetD3DViewport( ) const
   {
      D3D11_VIEWPORT viewport;
      ZeroMemory( &viewport, sizeof( viewport ) );

      viewport.Width = m_width;
      viewport.Height = m_height;
      viewport.MinDepth = m_minDepth;
      viewport.MaxDepth = m_maxDepth;
      viewport.TopLeftX = m_topLeftX;
      viewport.TopLeftY = m_topLeftY;
      return viewport;
   }

   bool Viewport::Bind( ID3D11DeviceContext& const deviceContext )
   {
      if ( m_renderer == nullptr )
      {
         return false;
      }

      D3D11_VIEWPORT viewport = GetD3DViewport( );
      deviceContext.RSSetViewports( 1, &viewport );

      return true;
   }
}