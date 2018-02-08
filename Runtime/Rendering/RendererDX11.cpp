#include "RendererDX11.h"
#include "RasterizerState.h"
#include "DepthStencilBufferDX11.h"
#include "RenderTargetDX11.h"
#include "Mesh.h"
#include "Quad.h"
#include "GBuffer.h"
#include "GBufferPass.h"
#include "LightBufferPass.h"
#include "ShadingPass.h"
#include "TestRenderPass.h"
#include "CheckerBoardInterpolatePass.h"
#include "Core\Context.h"
#include "Core\Window.h"
#include "Core\World.h"
#include "Core\Entity.h"
#include "Core\Transform.h"
#include "Core\Logger.h"
#include "Math\Vector2.h"
#include "Component\MeshRenderComponent.h"
#include "Component\LightComponent.h"
#include "Component\CameraComponent.h"
#include "Resource\Material.h"
#include "Resource\Texture2D.h"
#include "MT\ThreadPool.h"

namespace Mile
{
   RendererDX11::RendererDX11( Context* context ) : 
      SubSystem( context ),
      m_window( nullptr ), m_clearColor{ 0.0f, 0.0f, 0.0f, 1.0f },
      m_device( nullptr ), m_immediateContext( nullptr ),
      m_deferredContexts{ nullptr, nullptr, nullptr, nullptr },
      m_swapChain( nullptr ), m_renderTargetView( nullptr ),
      m_depthStencilBuffer( nullptr ), m_bDepthStencilEnabled( true ),
      m_gBuffer( nullptr ), m_gBufferPass( nullptr ),
      m_lightBuffer( nullptr ), m_lightBufferPass( nullptr ),
      m_testPass( nullptr ),
      m_bCheckerBoardRenderingEnabled( false ),
      m_checkerBoard(nullptr), m_checkerBoardInterpolatePass( nullptr ),
      m_shadingPass( nullptr ),
      m_mainCamera( nullptr ),
      m_viewport( nullptr ),
      m_defaultState( nullptr )
   {
   }

   RendererDX11::~RendererDX11( )
   {
      DeInit( );
   }

   bool RendererDX11::Init( )
   {
      if ( m_context == nullptr || m_bIsInitialized )
      {
         MELog( m_context, TEXT( "RendererDX11" ), ELogType::WARNING, TEXT( "RendererDX11 already initialized." ), true );
         return false;
      }

      /* Initialize low level rendering api **/
      m_window = m_context->GetSubSystem<Window>( );
      if ( m_window == nullptr )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ), 
                ELogType::FATAL,
                TEXT( "Cannot found Window subsystem from Context." ), true );
         return false;
      }

      if ( !CreateDeviceAndSwapChain( ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create Device and SwapChain." ), true );
         return false;
      }

      if ( !CreateDepthStencilBuffer( ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create Depth-Stencil Buffer." ), true );
         return false;
      }

      m_backBuffer = new RenderTargetDX11( this );
      if ( !m_backBuffer->Init( m_renderTargetView, m_depthStencilBuffer ) )
      {
         return false;
      }

      /* Initialize Pre Light Pass **/
      m_screenQuad = new Quad( this );
      if ( !m_screenQuad->Init( -1.0f, -1.0f, 1.0f, 1.0f ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create screen quad." ), true );
         return false;
      }

      // @TODO: Multiple viewports
      Vector2 screenRes{ m_window->GetResolution( ) };
      m_viewport = new Viewport( this );
      m_viewport->SetWidth( screenRes.x );
      m_viewport->SetHeight( screenRes.y );

      m_defaultState = new RasterizerState( this );
      if ( !m_defaultState->Init( ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create Default RasteirzerState." ), true );
         return false;
      }

      m_gBuffer = new GBuffer( this );
      if ( !m_gBuffer->Init(
         static_cast< unsigned int >( screenRes.x ),
         static_cast< unsigned int >( screenRes.y ) ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create GBuffer." ), true );
         return false;
      }
      m_gBuffer->SetDepthStencilBuffer( m_depthStencilBuffer );

      m_gBufferPass = new GBufferPass( this );
      if ( !m_gBufferPass->Init( TEXT( "Contents/Shaders/GBuffer.hlsl" ) ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create GBuffer Rendering Pass." ), true );
         return false;
      }
      m_gBufferPass->SetGBuffer( m_gBuffer );

      m_lightBuffer = new RenderTargetDX11( this );
      if ( !m_lightBuffer->Init(
         static_cast< unsigned int >( screenRes.x ),
         static_cast< unsigned int >( screenRes.y ) ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create Light Buffer." ), true );
         return false;
      }

      m_lightBufferPass = new LightBufferPass( this );
      if ( !m_lightBufferPass->Init( TEXT( "Contents/Shaders/LightBuffer.hlsl" ) ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create Light Buffer Rendering Pass." ), true );
         return false;
      }
      m_lightBufferPass->SetGBuffer( m_gBuffer );
      m_lightBufferPass->SetCheckerBoardBuffer( m_gBufferPass->GetCheckerBoardConstantBuffer( ) );
      m_lightBufferPass->SetLightBuffer( m_lightBuffer );

      m_shadingPass = new ShadingPass( this );
      if ( !m_shadingPass->Init( TEXT( "Contents/Shaders/Shading.hlsl" ) ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create Shading Pass." ), true );
         return false;
      }
      m_shadingPass->SetLightBuffer( m_lightBuffer );
      m_shadingPass->SetCheckerBoardBuffer( m_gBufferPass->GetCheckerBoardConstantBuffer( ) );
      m_shadingPass->AcquireTransformBuffer( m_gBufferPass );
      m_shadingPass->SetRenderTarget( m_backBuffer );


      /* Initialize Checker Board Rendering **/
      m_checkerBoard = new RenderTargetDX11( this );
      if ( !m_checkerBoard->Init(
           static_cast< unsigned int >( screenRes.x ),
           static_cast< unsigned int >( screenRes.y ) ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create checker board." ), true );
         return false;
      }

      m_checkerBoardInterpolatePass = new CheckerBoardInterpolatePass( this );
      if ( !m_checkerBoardInterpolatePass->Init( TEXT( "Contents/Shaders/CheckerBoardInterpolate.hlsl" ) ) )
      {
         return false;
      }
      m_checkerBoardInterpolatePass->SetCheckerBoard( m_checkerBoard );
      m_checkerBoardInterpolatePass->SetRenderTarget( m_backBuffer );
      // @TODO: Set Shading Pass Render Target depending on checker board settings

      /* Initialize Test render pass **/
      m_testPass = new TestRenderPass( this );
      if ( !m_testPass->Init( TEXT( "Contents/Shaders/TestShader.hlsl" ) ) )
      {
         MELog( m_context,
                TEXT( "RendererDX11" ),
                ELogType::FATAL,
                TEXT( "Failed to create Test Rendering Pass." ), true );
         return false;
      }

      MELog( m_context, TEXT( "RendererDX11" ), ELogType::MESSAGE, TEXT( "RendererDX11 Initialized!" ), true );
      m_bIsInitialized = true;
      return true;
   }

   void RendererDX11::DeInit( )
   {
      if ( m_bIsInitialized )
      {
         SafeDelete( m_viewport );
         SafeDelete( m_defaultState );
         SafeDelete( m_screenQuad );
         SafeDelete( m_testPass );
         SafeDelete( m_shadingPass );
         SafeDelete( m_lightBufferPass );
         SafeDelete( m_lightBuffer );
         SafeDelete( m_gBufferPass );
         SafeDelete( m_gBuffer );
         SafeRelease( m_renderTargetView );
         SafeDelete( m_depthStencilBuffer );
         SafeRelease( m_swapChain );

         for ( auto deferredContext : m_deferredContexts )
         {
            SafeRelease( deferredContext );
         }

         SafeRelease( m_immediateContext );
         SafeRelease( m_device );

         SubSystem::DeInit( );
         MELog( m_context, TEXT( "RendererDX11" ), ELogType::MESSAGE, TEXT( "RendererDX11 deinitialized." ), true );
      }
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
                                               &m_immediateContext );

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

      // Create Deferred Contexts
      for ( auto idx = 0;
            idx < static_cast< uint32_t >( EDeviceContextType::EnumSize ) - 1;
            ++idx )
      {
         hr = m_device->CreateDeferredContext( 0, &m_deferredContexts[ idx ] );
         if ( FAILED( hr ) )
         {
            // Failed to create deferred contexts.
            return false;
         }
      }

      return true;
   }

   bool RendererDX11::CreateDepthStencilBuffer( )
   {
      if ( m_device == nullptr )
      {
         return false;
      }

      m_depthStencilBuffer = new DepthStencilBufferDX11( this );
      bool res = m_depthStencilBuffer->Init( m_window->GetResWidth( ),
                                             m_window->GetResHeight( ),
                                             true );

      if ( !res )
      {
         return false;
      }

      return true;
   }

   void RendererDX11::AcquireMeshRenderersAndMaterial( const std::vector<Entity*>& entities )
   {
      m_meshRenderComponents.clear( );
      m_materialMap.clear( );
      for ( auto entity : entities )
      {
         auto meshRenderComponent = entity->GetComponent<MeshRenderComponent>( );
         if ( meshRenderComponent != nullptr )
         {
            if ( entity->IsActive( ) && meshRenderComponent->IsActive( ) )
            {
               auto material = meshRenderComponent->GetMaterial( );
               if ( material != nullptr )
               {
                  // Material Batching
                  m_meshRenderComponents.push_back( meshRenderComponent );
                  m_materialMap[ material ].push_back( meshRenderComponent );
               }
            }
         }
      }
   }

   void RendererDX11::AcquireLights( const std::vector<Entity*>& entities )
   {
      m_lightComponents.clear( );
      for ( auto entity : entities )
      {
         auto lightComponent = entity->GetComponent<LightComponent>( );
         if ( lightComponent != nullptr )
         {
            if ( entity->IsActive( ) && lightComponent->IsActive( ) )
            {
               m_lightComponents.push_back( lightComponent );
            }
         }
      }
   }

   void RendererDX11::AcquireCameras( const std::vector<Entity*>& entities )
   {
      m_cameras.clear( );
      for ( auto entity : entities )
      {
         auto cameraComponent = entity->GetComponent<CameraComponent>( );
         if ( cameraComponent != nullptr )
         {
            if ( entity->IsActive( ) && cameraComponent->IsActive( ) )
            {
               m_cameras.push_back( cameraComponent );
            }
         }
      }
   }

   void RendererDX11::Render( )
   {
      auto threadPool = m_context->GetSubSystem<ThreadPool>( );
      //Clear( *m_immediateContext );

      World* world = m_context->GetSubSystem<World>( );
      if ( world != nullptr )
      {
         std::vector<Entity*> entities = world->GetEntities( );

         // Acquire necessarry informations
         auto acquireMeshRenderersAndMatBinder = std::bind( &RendererDX11::AcquireMeshRenderersAndMaterial,
                                                            this, 
                                                            entities );
         auto acquireMeshRenderersAndMatTask = threadPool->AddTask( acquireMeshRenderersAndMatBinder );

         auto acquireLightBinder = std::bind( &RendererDX11::AcquireLights, this, entities );
         auto acquireLightTask = threadPool->AddTask( acquireLightBinder );

         auto acquireCamerasBinder = std::bind( &RendererDX11::AcquireCameras, this, entities );
         auto acquireCamerasTask = threadPool->AddTask( acquireCamerasBinder );

         acquireMeshRenderersAndMatTask.get( );
         acquireLightTask.get( );
         acquireCamerasTask.get( );

         if ( !m_cameras.empty( ) )
         {
            // @TODO: Implement Multiple camera rendering
            m_mainCamera = m_cameras[ 0 ];

            SetClearColor( m_mainCamera->GetClearColor( ) );

            auto threadPool = m_context->GetSubSystem<ThreadPool>( );

            /* Light Pre Pass **/
            auto gBufferPassBinder = std::bind( &RendererDX11::RenderGBuffer,
                                                this,
                                                GetDeviceContextByType( EDeviceContextType::GBufferPass ) );
            auto gBufferPassTask = threadPool->AddTask( gBufferPassBinder );

            auto lBufferPassBinder = std::bind( &RendererDX11::RenderLightBuffer,
                                                this,
                                                GetDeviceContextByType( EDeviceContextType::LBufferPass ) );
            auto lBufferPassTask = threadPool->AddTask( lBufferPassBinder );

            auto checkerBoardPassBinder = std::bind( &RendererDX11::RenderCheckerBoardInterpolate,
                                                     this,
                                                     GetDeviceContextByType( EDeviceContextType::CheckerBoardInterpolatePass ) );
            auto checkerBoardPassTask = threadPool->AddTask( checkerBoardPassBinder );

            ID3D11CommandList* gBufferPassCommandList = gBufferPassTask.get( );
            ID3D11CommandList* lBufferPassCommandList = lBufferPassTask.get( );
            ID3D11CommandList* checkerBoardCommandList = checkerBoardPassTask.get( );

            // Has dependency with G-Buffer
            auto shadingPassBinder = std::bind( &RendererDX11::RenderShading,
                                                this,
                                                GetDeviceContextByType( EDeviceContextType::ShadingPass ) );
            auto shadingPassTask = threadPool->AddTask( shadingPassBinder );

            ID3D11CommandList* shadingPassCommandList = shadingPassTask.get( );

            m_immediateContext->ExecuteCommandList( gBufferPassCommandList, false );
            m_immediateContext->ExecuteCommandList( lBufferPassCommandList, false );
            m_immediateContext->ExecuteCommandList( shadingPassCommandList, false );

            if ( checkerBoardCommandList != nullptr )
            {
               m_immediateContext->ExecuteCommandList( checkerBoardCommandList, false );
            }

            SafeRelease( gBufferPassCommandList );
            SafeRelease( lBufferPassCommandList );
            SafeRelease( shadingPassCommandList );
            SafeRelease( checkerBoardCommandList );
         }
      }

      Present( );
   }

   ID3D11CommandList* RendererDX11::RenderGBuffer( ID3D11DeviceContext* deviceContext )
   {
      deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
      m_defaultState->Bind( ( *deviceContext ) );
      m_viewport->Bind( ( *deviceContext ) );
      m_gBufferPass->UpdateCheckerBoardBuffer( *deviceContext, m_bCheckerBoardRenderingEnabled );
      m_gBufferPass->Bind( *deviceContext );

      auto camTransform = m_mainCamera->GetTransform( );

      for ( auto batchedMaterial : m_materialMap )
      {
         auto material = batchedMaterial.first;
         auto normalTexture = material->GetNormalMap( );
         m_gBufferPass->UpdateMaterialBuffer( *deviceContext,
                                              material->GetSpecularExp( ) ); // per material
         m_gBufferPass->UpdateNormalTexture( *deviceContext,
                                             normalTexture->GetRawTexture( ) ); // per material

         for ( auto meshRenderer : batchedMaterial.second )
         {
            auto transform = meshRenderer->GetEntity( )->GetTransform( );
            auto mesh = meshRenderer->GetMesh( );
            
            Matrix world = transform->GetWorldMatrix( );
            Matrix view = Matrix::CreateView(
               camTransform->GetPosition( TransformSpace::World ),
               camTransform->GetForward( ),
               camTransform->GetUp( ) );
            Matrix proj = Matrix::CreatePerspectiveProj(
               m_mainCamera->GetFov( ),
               m_window->GetAspectRatio( ),
               m_mainCamera->GetNearPlane( ),
               m_mainCamera->GetFarPlane( ) );

            Matrix worldView = world * view;
            Matrix worldViewProj = worldView * proj;

            m_gBufferPass->UpdateTransformBuffer( *deviceContext,
                                                  world,
                                                  worldView,
                                                  worldViewProj );  // per object

            // @TODO: Implement instancing
            mesh->Bind( *deviceContext, 0 );
            deviceContext->DrawIndexed( mesh->GetIndexCount( ), 0, 0 );
         }
      }

      // End of gbuffer pass
      m_gBufferPass->Unbind( *deviceContext );

      ID3D11CommandList* commandList = nullptr;
      auto hr = deviceContext->FinishCommandList( false, &commandList );
      if ( FAILED( hr ) )
      {
         return nullptr;
      }

      return commandList;
   }

   ID3D11CommandList* RendererDX11::RenderLightBuffer( ID3D11DeviceContext* deviceContext )
   {
      deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
      m_defaultState->Bind( ( *deviceContext ) );
      m_viewport->Bind( ( *deviceContext ) );
      m_lightBufferPass->Bind( *deviceContext );

      // @TODO: Implement multi - camera rendering
      auto cameraTransform = m_cameras[ 0 ]->GetTransform( );
      m_lightBufferPass->UpdateCameraBuffer( *deviceContext,
                                             cameraTransform->GetPosition( TransformSpace::World ) );

      for ( auto light : m_lightComponents )
      {
         m_lightBufferPass->UpdateLightParamBuffer(
            *deviceContext,
            light->GetLightPosition( ),
            light->GetLightColor( ),
            light->GetLightDirection( ),
            light->GetSpotlightAngles( ),
            Vector3( light->GetLightRange( ), 0.0f, 0.0f ),
            LightComponent::LightTypeToIndex( light->GetLightType( ) ) );

         m_screenQuad->Bind( *deviceContext, 0 );
         deviceContext->DrawIndexed( 6, 0, 0 );
      }

      m_lightBufferPass->Unbind( *deviceContext );

      ID3D11CommandList* commandList = nullptr;
      auto hr = deviceContext->FinishCommandList( false, &commandList );
      if ( FAILED( hr ) )
      {
         return nullptr;
      }

      return commandList;
   }

   ID3D11CommandList* RendererDX11::RenderShading( ID3D11DeviceContext* deviceContext )
   {
      deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
      m_defaultState->Bind( ( *deviceContext ) );
      m_viewport->Bind( ( *deviceContext ) );

      if ( m_bCheckerBoardRenderingEnabled )
      {
         m_checkerBoard->SetClearColor( m_clearColor );
         m_shadingPass->SetRenderTarget( m_checkerBoard );
      }
      else
      {
         m_backBuffer->SetClearColor( m_clearColor );
         m_shadingPass->SetRenderTarget( m_backBuffer );
      }

      m_shadingPass->Bind( *deviceContext );

      auto camTransform = m_mainCamera->GetTransform( );

      for ( auto batchedMaterial : m_materialMap )
      {
         auto material = batchedMaterial.first;
         auto diffuseTexture = material->GetDiffuseMap( );
         m_shadingPass->UpdateMaterialBuffer( *deviceContext,
                                              material->GetSpecularAlbedo( ) );
         m_shadingPass->UpdateDiffuseTexture( *deviceContext,
                                              diffuseTexture->GetRawTexture( ) );

         for ( auto meshRenderer : batchedMaterial.second )
         {
            auto transform = meshRenderer->GetTransform( );
            auto mesh = meshRenderer->GetMesh( );

            Matrix world = transform->GetWorldMatrix( );
            Matrix view = Matrix::CreateView(
               camTransform->GetPosition( TransformSpace::World ),
               camTransform->GetForward( ),
               camTransform->GetUp( ) );
            Matrix proj = Matrix::CreatePerspectiveProj(
               m_mainCamera->GetFov( ),
               m_window->GetAspectRatio( ),
               m_mainCamera->GetNearPlane( ),
               m_mainCamera->GetFarPlane( ) );

            Matrix worldView = world * view;
            Matrix worldViewProj = worldView * proj;


            m_shadingPass->UpdateTransformBuffer( *deviceContext,
                                                  world,
                                                  worldView,
                                                  worldViewProj );  // per object

            // @TODO: Implement instancing
            mesh->Bind( *deviceContext, 0 );
            deviceContext->DrawIndexed( mesh->GetIndexCount( ), 0, 0 );
         }

         // Unbind ShaderResource
         m_shadingPass->UpdateDiffuseTexture( *deviceContext, nullptr );
      }

      m_shadingPass->Unbind( *deviceContext );

      ID3D11CommandList* commandList = nullptr;
      auto hr = deviceContext->FinishCommandList( false, &commandList );
      if ( FAILED( hr ) )
      {
         return nullptr;
      }

      return commandList;
   }

   ID3D11CommandList* RendererDX11::RenderCheckerBoardInterpolate( ID3D11DeviceContext* deviceContext )
   {
      if ( !m_bCheckerBoardRenderingEnabled )
      {
         return nullptr;
      }

      deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
      m_defaultState->Bind( ( *deviceContext ) );
      m_viewport->Bind( ( *deviceContext ) );
      m_checkerBoardInterpolatePass->Bind( *deviceContext );

      Vector2 screenRes{ m_window->GetResolution( ) };
      m_checkerBoardInterpolatePass->UpdateRenderTargetInfoBuffer( *deviceContext,
                                                                   static_cast< int >( screenRes.x ),
                                                                   static_cast< int >( screenRes.y ) );

      m_screenQuad->Bind( *deviceContext, 0 );
      deviceContext->DrawIndexed( 6, 0, 0 );

      m_checkerBoardInterpolatePass->Unbind( *deviceContext );

      ID3D11CommandList* commandList = nullptr;
      auto hr = deviceContext->FinishCommandList( false, &commandList );
      if ( FAILED( hr ) )
      {
         return nullptr;
      }

      return commandList;
   }

   void RendererDX11::RenderTest( ID3D11DeviceContext& deviceContext )
   {
      deviceContext.IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
      m_defaultState->Bind( ( deviceContext ) );
      m_viewport->Bind( ( deviceContext ) );

      m_testPass->Bind( deviceContext );
      SetBackbufferAsRenderTarget( deviceContext );

      auto camTransform = m_mainCamera->GetTransform( );

      for ( auto batchedMaterial : m_materialMap )
      {
         auto material = batchedMaterial.first;
         auto diffuseTexture = material->GetDiffuseMap( );
         m_testPass->UpdateDiffuseMap( deviceContext,
                                       diffuseTexture->GetRawTexture( ) );

         for ( auto meshRenderer : batchedMaterial.second )
         {
            auto transform = meshRenderer->GetTransform( );
            auto mesh = meshRenderer->GetMesh( );

            Matrix world = transform->GetWorldMatrix( );
            Matrix worldView = world *
               Matrix::CreateView(
                  camTransform->GetPosition( TransformSpace::World ),
                  camTransform->GetForward( ) );
            Matrix worldViewProj = worldView *
               Matrix::CreatePerspectiveProj(
                  m_mainCamera->GetFov( ),
                  m_window->GetAspectRatio( ),
                  m_mainCamera->GetNearPlane( ),
                  m_mainCamera->GetFarPlane( ) );
            m_testPass->UpdateTransformBuffer( deviceContext, 
                                               world,
                                               worldView,
                                               worldViewProj );  // per object

            mesh->Bind( deviceContext, 0 );
            deviceContext.DrawIndexed( mesh->GetIndexCount( ), 0, 0 );
         }

         // Unbind ShaderResource
         m_testPass->UpdateDiffuseMap( deviceContext, nullptr );
      }

      m_testPass->Unbind( deviceContext );
   }

   void RendererDX11::Clear( ID3D11DeviceContext& deviceContext )
   {
      float clearColor[ 4 ] = { m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w };
      deviceContext.ClearRenderTargetView( m_renderTargetView,
                                           clearColor );

      ClearDepthStencil( deviceContext );
   }

   void RendererDX11::ClearDepthStencil( ID3D11DeviceContext& deviceContext )
   {
      if ( m_bDepthStencilEnabled )
      {
         deviceContext.ClearDepthStencilView(
            m_depthStencilBuffer->GetDSV( ),
            D3D11_CLEAR_DEPTH, 1.0f, 0 );
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

   void RendererDX11::SetDepthStencilEnable( ID3D11DeviceContext& deviceContext, bool bDepthStencilEnable )
   {
      m_bDepthStencilEnabled = bDepthStencilEnable;
      SetBackbufferAsRenderTarget( deviceContext  );
   }

   void RendererDX11::SetBackbufferAsRenderTarget( ID3D11DeviceContext& deviceContext )
   {
      if ( m_bDepthStencilEnabled )
      {
         deviceContext.OMSetRenderTargets( 1, &m_renderTargetView, m_depthStencilBuffer->GetDSV( ) );
      }
      else
      {
         deviceContext.OMSetRenderTargets( 1, &m_renderTargetView, nullptr );
      }
   }

   ID3D11DeviceContext* RendererDX11::GetDeviceContextByType( EDeviceContextType type )
   {
      if ( type == EDeviceContextType::Immediate )
      {
         return ( m_immediateContext );
      }
      switch ( type )
      {
      case EDeviceContextType::Immediate:
      default:
         return m_immediateContext;
      case EDeviceContextType::GBufferPass:
         return m_deferredContexts[ 0 ];
      case EDeviceContextType::LBufferPass:
         return m_deferredContexts[ 1 ];
      case EDeviceContextType::ShadingPass:
         return m_deferredContexts[ 2 ];
      case EDeviceContextType::CheckerBoardInterpolatePass:
         return m_deferredContexts[ 3 ];
      }
   }

}