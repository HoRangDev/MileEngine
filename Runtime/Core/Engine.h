#pragma once

#include "SubSystem.h"

namespace Mile
{
   class ConfigSystem;
   class Window;
   class ResourceManager;
   class World;
   class RendererDX11;
   class MEAPI Engine : public SubSystem
   {
   public:
      Engine( Context* context );
      virtual ~Engine( ) { ShutDown( ); }

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

      bool IsRunning( ) const { return m_bIsRunning; }

   private:
      bool                m_bIsRunning;
      ConfigSystem*       m_configSys;
      Window*             m_window;
      ResourceManager*    m_resourceManager;
      RendererDX11*       m_renderer;
      World*              m_world;

   };
}