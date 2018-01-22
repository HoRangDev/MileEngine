#include "Texture2D.h"
#include "TextureLoader.h"
#include "Rendering\Texture2dDX11.h"
#include "Rendering\RendererDX11.h"
#include "Core\Context.h"
#include "Core\Logger.h"

namespace Mile
{
   Texture2D::Texture2D( Context* context, const String& filePath ) :
      m_rawData( nullptr ),
      m_rawTexture( nullptr ),
      m_width( 0 ),
      m_height( 0 ),
      m_channels( 0 ),
      m_bitDepth( 0 ),
      Resource( context, filePath,
                ResourceType::RT_Texture2D)
   {
   }

   Texture2D::~Texture2D( )
   {
      SafeDelete( m_rawTexture );
      SafeArrayDelete( m_rawData );
   }

   bool Texture2D::Init( )
   {
      if ( m_context == nullptr ||  m_bIsInitialized )
      {
         MELog( m_context, TEXT( "Texture2D" ), ELogType::WARNING, TEXT( "Is already initialized." ), true );
         return false;
      }

      auto info = TextureLoader::LoadTexture( m_path, m_ext );
      m_rawData = std::get<TextureInfoTag::TIT_RAWDATA>( info );
      m_width = std::get<TextureInfoTag::TIT_WIDTH>( info );
      m_height = std::get<TextureInfoTag::TIT_HEIGHT>( info );
      m_channels = std::get<TextureInfoTag::TIT_CHANNELS>( info );
      
      if ( m_rawData == nullptr )
      {
         MELog( m_context, TEXT( "Texture2D" ), ELogType::WARNING, TEXT( "Failed to load Texture2D from " ) + m_path, true );
         return false;
      }

      if ( !InitRawTexture( ) )
      {
         SafeArrayDelete( m_rawData );
         return false;
      }

      m_bIsInitialized = true;
      return true;
   }

   bool Texture2D::Save( const String& filePath )
   {
      return false;
   }

   bool Texture2D::InitRawTexture( )
   {
      if ( m_rawData == nullptr || m_rawTexture != nullptr )
      {
         return false;
      }
      
      auto renderer = m_context->GetSubSystem<RendererDX11>( );
      m_rawTexture = new Texture2dDX11( renderer );
      if ( !m_rawTexture->Init( m_width, m_height, m_channels, m_rawData, DXGI_FORMAT_B8G8R8A8_UNORM ) )
      {
         MELog( m_context, TEXT( "Texture2D" ), ELogType::WARNING, TEXT( "Failed to initialize Raw Texture: " ) + m_path, true );
         SafeDelete( m_rawTexture );
         return false;
      }
      
      return true;
   }
}