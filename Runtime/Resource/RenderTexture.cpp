#include "Resource/RenderTexture.h"
#include "Core/Context.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Rendering/RendererDX11.h"
#include "Rendering/RenderTargetDX11.h"
#include "Rendering/DepthStencilBufferDX11.h"
#include "json.hpp"

namespace Mile
{
   bool RenderTexture::Init()
   {
      if (m_context == nullptr || m_bIsInitialized)
      {
         MELog(m_context, TEXT("RenderTexture"), ELogType::WARNING, TEXT("Already intiiailized RenderTexture or Context does not exist!"));
         return false;
      }

      std::ifstream stream(this->m_path);
      if (!stream.is_open())
      {
         MELog(m_context, TEXT("RenderTexture"), ELogType::WARNING, TEXT("Failed to load render texture from ") + m_path);
         return false;
      }

      std::string jsonStr;
      std::string temp;
      while (std::getline(stream, temp))
      {
         jsonStr += temp;
         jsonStr += '\n';
      }
      stream.close();

      this->DeSerialize(json::parse(jsonStr));
      return true;
   }

   bool RenderTexture::SaveTo(const String& filePath)
   {
      json serialized = this->Serialize();
      std::ofstream stream(filePath);
      stream << serialized.dump();
      stream.close();

      return true;
   }

   RenderTargetDX11* RenderTexture::GetRenderTarget()
   {
      if (m_bIsDirty)
      {
         GenerateRenderTarget();
      }

      return m_renderTarget;
   }

   json RenderTexture::Serialize() const
   {
      json serialized = Resource::Serialize();
      serialized["Width"] = m_width;
      serialized["Height"] = m_height;
      serialized["ColorFormat"] = static_cast<UINT32>(m_colorFormat);
      serialized["EnableStencil"] = m_bEnableStencil;

      return json();
   }

   void RenderTexture::DeSerialize(const json& jsonData)
   {
      m_width = GetValueSafelyFromJson<UINT32>(jsonData, "Width", 1);
      m_height = GetValueSafelyFromJson<UINT32>(jsonData, "Height", 1);
      m_colorFormat = static_cast<EColorFormat>(
            GetValueSafelyFromJson<UINT32>(
               jsonData, 
               "ColorFormat", 
               static_cast<UINT32>(EColorFormat::R8G8B8A8_UNORM)));
      m_bEnableStencil = GetValueSafelyFromJson<bool>(jsonData, "EnableStencil", true);
   }

   void RenderTexture::GenerateRenderTarget()
   {
      RendererDX11* renderer = Engine::GetRenderer();
      if (renderer != nullptr)
      {
         m_depthStencil = new DepthStencilBufferDX11(renderer);
         if (!m_depthStencil->Init(m_width, m_height, m_bEnableStencil))
         {
            MELog(m_context, TEXT("RenderTexture"), ELogType::FATAL, TEXT("Failed to initialize depth stencil"));
            return;
         }

         m_renderTarget = new RenderTargetDX11(renderer);
         if (m_renderTarget->Init(m_width, m_height, ColorFormatToDXGIFormat(m_colorFormat)))
         {
            MELog(m_context, TEXT("RenderTexture"), ELogType::FATAL, TEXT("Failed to initialize render target"));
            return;
         }
      }
      else
      {
         MELog(m_context, TEXT("RenderTexture"), ELogType::WARNING, TEXT("Renderer doest not availiable!"));
      }
   }
}