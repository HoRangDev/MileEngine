#include "Layers/GameViewLayer.h"
#include "Core/imguiHelper.h"
#include "Core/Window.h"
#include "Resource/ResourceManager.h"
#include "Resource/RenderTexture.h"
#include "Rendering/RenderTargetDX11.h"
#include "Rendering/Texture2dDX11.h"
#include "Rendering/RendererDX11.h"
#include "Component/CameraComponent.h"
#include "GameFramework/World.h"

namespace Mile
{
   namespace Editor
   {
      constexpr float GameViewDefaultWidth = 1280.0f;
      constexpr float GameViewDefaultHeight = 720.0f;
      constexpr float GameViewDefaultFOV = 45.0f;
      GameViewLayer::GameViewLayer(Context* context) :
         m_editorCameraRenderTex(nullptr),
         Layer(context)
      {
      }

      bool GameViewLayer::Init()
      {
         auto resMng = Engine::GetResourceManager();
         auto world = Engine::GetWorld();
         m_editorCameraRenderTex = resMng->Create<RenderTexture>(EDITOR_GAME_VIEW_RENDER_TEXTURE, false);
         m_editorCameraRenderTex->SetWidth(GameViewDefaultWidth);
         m_editorCameraRenderTex->SetHeight(GameViewDefaultHeight);
         //m_editorCamera = world->CreateEntity(TEXT("GameViewCamera"));
         //m_editorCamera->SetVisibleOnHierarchy(true);
         //m_editorCamera->SetSerializable(false);
         //m_editorCameraComponent = m_editorCamera->AddComponent<CameraComponent>();
         //m_editorCameraComponent->SetRenderTexture(m_editorCameraRenderTex);

         return true;
      }

      void GameViewLayer::OnIMGUIRender()
      {
         auto world = Engine::GetWorld();
         auto window = Engine::GetWindow();
         Vector2 maxRes = window->GetResolution();

         ImGui::SetNextWindowSizeConstraints(ImVec2{ 50.0f, 50.0f }, ImVec2{ maxRes.x, maxRes.y });
         ImGui::Begin("Game");
         if (m_editorCameraRenderTex != nullptr)
         {
            auto contentRegion = ImGui::GetContentRegionAvail();

            if (ImGui::IsWindowFocused())
            {
               ImVec2 vMin;
               ImVec2 vMax = ImGui::GetWindowSize();

               ImVec2 winPos = ImGui::GetWindowPos();
               vMin.x += winPos.x;
               vMin.y += winPos.y;
               vMax.x += winPos.x;
               vMax.y += winPos.y;

               ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255), 0.0f, 15, 0.8f);
            }

            ImVec2 outputRes{ contentRegion.x, contentRegion.y };
            m_editorCameraRenderTex->SetWidth(outputRes.x);
            m_editorCameraRenderTex->SetHeight(outputRes.y);
            //m_editorCameraComponent->SetFov(GameViewDefaultFOV * ((outputRes.x + outputRes.y) / (GameViewDefaultWidth + GameViewDefaultHeight)));
            ImGui::Image((void*)m_editorCameraRenderTex->GetRenderTarget()->GetTexture()->GetSRV(), outputRes);
         }
         ImGui::End();
      }
   }
}

