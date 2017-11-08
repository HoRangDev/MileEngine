#include "RendererDX11.h"
#include "Core/Context.h"
#include "Core/Window.h"
#include "DepthStencilBufferDX11.h"

namespace Mile
{
   RendererDX11::RendererDX11( Context* context ) : SubSystem( context ),
      m_window( nullptr ),
      m_device( nullptr ), m_deviceContext( nullptr ),
      m_swapChain( nullptr ), m_renderTargetView( nullptr ),
      m_depthStencilBuffer( nullptr ), m_bDepthStencilEnabled( true )
   {
   }

   RendererDX11::~RendererDX11( )
   {
      SafeRelease( m_renderTargetView );
      m_depthStencilBuffer.reset( nullptr );
      SafeRelease( m_swapChain );
      SafeRelease( m_deviceContext );
      SafeRelease( m_device );
   }

   bool RendererDX11::Init( )
   {
      m_window = m_context->GetSubSystem<Window>( );
      if ( !CreateDeviceAndSwapChain( ) )
      {
         return false;
      }

      if ( !CreateDepthStencilBuffer( ) )
      {
         return false;
      }

      SetBackbufferAsRenderTarget( );
      return true;
   }

   bool RendererDX11::CreateDeviceAndSwapChain( )
   {
      DXGI_SWAP_CHAIN_DESC swDesc;
      ZeroMemory( &swDesc, sizeof( swDesc ) );

      swDesc.BufferCount = 1;
      swDesc.BufferDesc.Width = m_window->GetResWidth( );
      swDesc.BufferDesc.Height = m_window->GetResHeight( );
      swDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
      swDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      swDesc.OutputWindow = reinterpret_cast< HWND >( m_window->GetHandle( ) );
      swDesc.SampleDesc.Count = 1;
      swDesc.SampleDesc.Quality = 0;
      swDesc.Windowed = !m_window->IsFullScreen( );

      unsigned int createDeviceFlags = 0;
#ifdef _DEBUG
      createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

      D3D_FEATURE_LEVEL featureLevels[ ] = { D3D_FEATURE_LEVEL_11_0 };
      unsigned int numOfFeatureLevels = ARRAYSIZE( featureLevels );
      D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;


      auto hr = D3D11CreateDeviceAndSwapChain( nullptr,
                                               D3D_DRIVER_TYPE_HARDWARE,
                                               NULL,
                                               createDeviceFlags,
                                               featureLevels,
                                               numOfFeatureLevels,
                                               D3D11_SDK_VERSION,
                                               &swDesc,
                                               &m_swapChain,
                                               &m_device,
                                               &featureLevel,
                                               &m_deviceContext );

      if ( FAILED( hr ) )
      {
         /* Failed to create device and swap chain. **/
         return false;
      }

      /* Create Render target view **/
      ID3D11Texture2D* backBuffer = nullptr;
      hr = m_swapChain->GetBuffer( 0,
                                   __uuidof( ID3D11Texture2D ),
                                   ( LPVOID* ) &backBuffer );

      if ( FAILED( hr ) )
      {
         /* Failed to get back buffer texture resource from swap chain. **/
         return false;
      }

      hr = m_device->CreateRenderTargetView( backBuffer,
                                             nullptr,
                                             &m_renderTargetView );
      SafeRelease( backBuffer );

      if ( FAILED( hr ) )
      {
         /* Failed to create render target view with backbuffer. **/
         return false;
      }

      return true;
   }

   bool RendererDX11::CreateDepthStencilBuffer( )
   {
      if ( m_device == nullptr )
      {
         return false;
      }

      m_depthStencilBuffer = std::make_unique<DepthStencilBufferDX11>( this );
      bool res = m_depthStencilBuffer->Init( m_window->GetResWidth( ),
                                  m_window->GetResHeight( ),
                                  true );

      if ( !res )
      {
         return false;
      }

      return true;
   }

   void RendererDX11::Render( )
   {
   }

   void RendererDX11::Clear( )
   {
      if ( m_deviceContext != nullptr )
      {
         float clearColor[ 4 ] = { m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w };
         m_deviceContext->ClearRenderTargetView( m_renderTargetView,
                                                 clearColor );

         if ( m_bDepthEnabled )
         {
            m_deviceContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
         }
      }
   }

   void RendererDX11::Present( )
   {
      if ( m_swapChain != nullptr )
      {
         /* @TODO: Impl VSYNC **/
         m_swapChain->Present( 0, 0 );
      }
   }

   void RendererDX11::SetClearColor( Vector4 clearColor )
   {
      m_clearColor = clearColor;
   }

   void RendererDX11::SetDepthStencilEnable( bool bDepthStencilEnable )
   {
      m_bDepthStencilEnabled = bDepthStencilEnable;
      SetBackbufferAsRenderTarget( );
   }

   void RendererDX11::SetBackbufferAsRenderTarget( )
   {
      if ( m_bDepthStencilEnabled )
      {
         m_deviceContext->OMSetRenderTargets( 1, &m_renderTargetView, m_depthStencilBuffer->GetDSV( ) );
      }
      else
      {
         m_deviceContext->OMSetRenderTargets( 1, &m_renderTargetView, nullptr );
      }
   }
}