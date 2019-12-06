#pragma once

#include "SubSystem.h"

namespace Mile
{
   class Logger;
   class Timer;
   class ThreadPool;
   class ConfigSystem;
   class Window;
   class ResourceManager;
   class RendererDX11;
   class World;
   /**
    * @brief	��� Subsystem ���� root ������ �մϴ�. Subsystem ���� �ʱ�ȭ, ������Ʈ �׸��� �Ҵ� ������ ����ϴ� Ŭ���� �Դϴ�.
    */
   class MEAPI Engine : public SubSystem
   {
   public:
      Engine( Context* context );

      /**
      * @brief Engine�� Subsystem ���� �ʱ�ȭ�մϴ�.
      * @return �ʱ�ȭ ���� ����
      */
      virtual bool Init( ) override;

      int Execute( );

      /**
      * @brief Engine�� Subsystem ���� ������Ʈ�մϴ�.
      */
      void Update( );

      /**
      * @brief Engine�� Subsystem ���� �����մϴ�.
      */
      void ShutDown( );
      void ShutDownFlagEnable( ) { m_bShutdownFlag = true; }

      bool IsRunning( ) const { return m_bIsRunning; }

   private:
      bool                m_bIsRunning;
      bool                m_bShutdownFlag;
      unsigned int        m_maxFPS;
      long long           m_frameTime;
      Logger*             m_logger;
      Timer*              m_timer;
      ThreadPool*         m_threadPool;
      ResourceManager*    m_resourceManager;
      ConfigSystem*       m_configSys;
      Window*             m_window;
      RendererDX11*       m_renderer;
      World*              m_world;

   };
}