#include "Core/Engine.h"
#include "Core/Context.h"
#include "Core/Logger.h"
#include "Core/Timer.h"
#include "Core/Config.h"
#include "Core/Window.h"
#include "GameFramework/World.h"
#include "Resource/ResourceManager.h"
#include "Rendering/RendererDX11.h"
#include "MT/ThreadPool.h"
#include <iostream>
#include <algorithm>

namespace Mile
{
   Engine::Engine(Context* context) :
      SubSystem(context), m_bIsRunning(false), m_bShutdownFlag(false),
      m_maxFPS(0), m_targetTimePerFrame(0)
   {
      m_context->RegisterSubSystem(this);

      m_logger = new Logger(m_context);
      m_context->RegisterSubSystem(m_logger);

      m_timer = new Timer(m_context);
      m_context->RegisterSubSystem(m_timer);

      m_threadPool = new ThreadPool(m_context);
      m_context->RegisterSubSystem(m_threadPool);

      m_resourceManager = new ResourceManager(m_context);
      m_context->RegisterSubSystem(m_resourceManager);

      m_configSys = new ConfigSystem(m_context);
      m_context->RegisterSubSystem(m_configSys);

      m_window = new Window(m_context);
      m_context->RegisterSubSystem(m_window);

      m_renderer = new RendererDX11(m_context);
      m_context->RegisterSubSystem(m_renderer);

      m_world = new World(m_context);
      m_context->RegisterSubSystem(m_world);
   }

   bool Engine::Init()
   {
      if (m_context == nullptr)
      {
         return false;
      }

      // -* Initialize subsystems *-
      // Initialize Logger
      if (!m_context->GetSubSystem<Logger>()->Init())
      {
         return false;
      }

      // Initialize Timer
      if (!m_context->GetSubSystem<Timer>()->Init())
      {
         m_logger->Logging(TEXT("Engine"), ELogType::FATAL, TEXT("Timer failed to intiialize!"), true);
         return false;
      }

      // Initialize Thread Pool
      if (!m_context->GetSubSystem<ThreadPool>()->Init())
      {
         m_logger->Logging(TEXT("Engine"), ELogType::FATAL, TEXT("Thread Pool failed to intiialize!"), true);
         return false;
      }

      // Initialize Resource manager
      if (!m_context->GetSubSystem<ResourceManager>()->Init())
      {
         m_logger->Logging(TEXT("Engine"), ELogType::FATAL, TEXT("Resource Manager failed to intiialize!"), true);
         return false;
      }

      // Initialize ConfigSystem
      if (!m_context->GetSubSystem<ConfigSystem>()->Init())
      {
         m_logger->Logging(TEXT("Engine"), ELogType::FATAL, TEXT("Config System failed to intiialize!"), true);
         return false;
      }

      // Initialize Window Subsystem
      if (!m_context->GetSubSystem<Window>()->Init())
      {
         m_logger->Logging(TEXT("Engine"), ELogType::FATAL, TEXT("Resource Manager failed to intiialize!"), true);
         return false;
      }

      if (!m_context->GetSubSystem<RendererDX11>()->Init())
      {
         m_logger->Logging(TEXT("Engine"), ELogType::FATAL, TEXT("RendererDX11 failed to intiialize!"), true);
         return false;
      }

      // Initialize World
      if (!m_context->GetSubSystem<World>()->Init())
      {
         m_logger->Logging(TEXT("Engine"), ELogType::FATAL, TEXT("World failed to intiialize!"), true);
         return false;
      }

      auto engineConfig = m_configSys->GetConfig(TEXT("Engine"));
      m_maxFPS = std::clamp(static_cast<unsigned int>(engineConfig.second["MaxFPS"]), LOWER_BOUND_OF_ENGINE_FPS, UPPER_BOUND_OF_ENGINE_FPS);
      m_targetTimePerFrame = static_cast<long long>((1.0 / static_cast<double>(m_maxFPS)) * 1000.0);

      return true;
   }

   int Engine::Execute()
   {
      m_bIsRunning = true;
      m_world->Start();
      while (m_bIsRunning)
      {
         if (m_bShutdownFlag)
         {
            ShutDown();
         }
         else
         {
            m_timer->BeginFrame();

            this->Update();
            m_renderer->Render();

            auto deltaTimeMS = m_timer->GetDeltaTimeMS();
            if (deltaTimeMS < m_targetTimePerFrame)
            {
               std::this_thread::sleep_for(std::chrono::milliseconds(m_targetTimePerFrame - deltaTimeMS));
            }

            m_timer->EndFrame();
         }
      }

      return 0;
   }

   void Engine::Update()
   {
      // Update subsystems
      m_window->Update();
      m_world->Update();
   }

   void Engine::ShutDown()
   {
      SubSystem::DeInit();
      m_bIsRunning = false;
      m_logger = nullptr;
      m_timer = nullptr;
      m_threadPool = nullptr;
      m_resourceManager = nullptr;
      m_configSys = nullptr;
      m_window = nullptr;
      m_renderer = nullptr;
      m_world = nullptr;
      MELog(m_context, TEXT("Engine"), ELogType::MESSAGE, TEXT("Engine shutting down."), true);
   }
}
