#pragma once
#include "Core/Logger.h"

namespace Mile
{
   DECLARE_LOG_CATEGORY_EXTERN(MileEngine, ELogVerbosity::Log);

   constexpr unsigned int LOWER_BOUND_OF_ENGINE_FPS = 1;
   constexpr unsigned int UPPER_BOUND_OF_ENGINE_FPS = 300;

   class Timer;
   class ThreadPool;
   class ConfigSystem;
   class InputManager;
   class Window;
   class ResourceManager;
   class RendererDX11;
   class Application;
   class World;
   /**
    * @brief	��� Subsystem ���� root ������ �մϴ�. �������� ����� Subsystem ���� �ʱ�ȭ, ������Ʈ �׸��� �Ҵ� ������ ����ϴ� Ŭ���� �Դϴ�.
    */
   class MEAPI Engine : public SubSystem
   {
   public:
      Engine(Context* context, Application* app);
      ~Engine() = default;

      /**
      * @brief Engine�� Subsystem ���� �ʱ�ȭ�մϴ�.
      * @return �ʱ�ȭ ���� ����
      */
      virtual bool Init() override;

      int Execute();

      /**
      * @brief Engine�� Subsystem ���� ������Ʈ�մϴ�.
      */
      void Update();

      /**
      * @brief Engine�� Subsystem ���� �����մϴ�.
      */
      void ShutDown();
      void ShutDownFlagEnable() { m_bShutdownFlag = true; }

      bool IsRunning() const { return m_bIsRunning; }

      static Engine* GetInstance();
      static Logger* GetLogger();
      static Timer* GetTimer();
      static ThreadPool* GetThreadPool();
      static ResourceManager* GetResourceManager();
      static ConfigSystem* GetConfigSystem();
      static InputManager* GetInputManager();
      static Window* GetWindow();
      static RendererDX11* GetRenderer();
      static World* GetWorld();

   private:
      static Engine*    m_instance;
      bool              m_bIsRunning;
      bool              m_bShutdownFlag;
      unsigned int      m_maxFPS;
      long long         m_targetTimePerFrame;
      Logger*           m_logger;
      Timer*            m_timer;
      ThreadPool*       m_threadPool;
      ResourceManager*  m_resourceManager;
      ConfigSystem*     m_configSys;
      InputManager*     m_inputManager;
      Window*           m_window;
      RendererDX11*     m_renderer;
      World*            m_world;
      Application*      m_app;

   };
}
