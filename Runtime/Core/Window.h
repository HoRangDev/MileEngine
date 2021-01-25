#pragma once
#include "Core/Logger.h"
#include "Core/Delegate.h"
#include "Math/Vector2.h"
#include <Windows.h>

namespace Mile
{
   DECLARE_LOG_CATEGORY_EXTERN(MileWindow, Log);
   DECLARE_MULTICAST_DELEGATE_Params(OnWindowResizeMulticastDelegate, unsigned int, unsigned int);
   DECLARE_MULTICAST_DELEGATE(OnWindowMinimizedMulticastDelegate);
   DECLARE_DELEGATE_Params(OnWindowResizeDelegate, unsigned int, unsigned int);
   DECLARE_DELEGATE(OnWindowMinimizedDelegate);

   enum class EWindowStyle
   {
      FullScreen, // 0
      Borderless, // 1
      Windowed    // 2 and else
   };

   static EWindowStyle IndexToWindowStyle(unsigned int idx)
   {
      switch (idx)
      {
      case 0:
         return EWindowStyle::FullScreen;
      case 1:
         return EWindowStyle::Borderless;
      case 2:
      default:
         return EWindowStyle::Windowed;
      }
   }

   static unsigned long WindowStyleToFlags(EWindowStyle style)
   {
      auto flags = WS_POPUP;
      switch (style)
      {
      case Mile::EWindowStyle::FullScreen:
         break;
      case Mile::EWindowStyle::Borderless:
         break;
      case Mile::EWindowStyle::Windowed:
         flags |= WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
         break;
      }

      return flags;
   }

   static void WinSetVideoMode(int width, int height, int bpp)
   {
      DEVMODE Mode;
      EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &Mode);
      Mode.dmBitsPerPel = bpp;
      Mode.dmPelsWidth = width;
      Mode.dmPelsHeight = height;
      Mode.dmSize = sizeof(Mode);
      Mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

      ChangeDisplaySettings(&Mode, CDS_FULLSCREEN);
   }

   static void WinRestoreVideoMode()
   {
      ChangeDisplaySettings(NULL, 0);
   }

   /**
    * @brief	ȭ���� ������ �����츦 ��Ÿ���� ���� �ý��� Ŭ�����Դϴ�.
    */
   class MEAPI Window : public SubSystem
   {
   public:
      Window(Context* context);
      virtual ~Window();

      virtual bool Init() override;
      virtual void DeInit() override;

      /**
       * @brief	Win32�� raw handle �����͸� ��ȯ�մϴ�.
       * @return	�ڵ� ������
       */
      void* GetHandle() { return m_handle; }

      /**
       * @brief ������ �������� client ������ �ػ󵵸� ��ȯ�մϴ�.
       * @return �������� client ������ �ػ�
       */
      Vector2 GetResolution() const { return Vector2(static_cast<float>(m_resWidth), static_cast<float>(m_resHeight)); }

      /**
       * @brief ������ �������� client ������ �ػ� ��Ⱦ�� ��ȯ�մϴ�.
       * @return �������� client ������ �ػ� ��Ⱦ��
       */
      float GetAspectRatio() const { return (m_resWidth / static_cast<float>(m_resHeight)); }

      /**
      * @brief ������ ������ â�� �ػ󵵸� ��ȯ�մϴ�.
      * @return �������� client ������ �ػ�
      */
      Vector2 GetWindowResolution() const;
      float GetWindowAspectRatio() const;

      /**
       * @brief	������ ������ â�� ����󵵼��� ��ȯ�մϴ�.
       * @return ������ â�� ����󵵼�
       */
      Float GetRefreshRate() const { return m_refreshRate; }

      /**
       * @brief ������ â�� Ǯ ��ũ�� ������� Ȯ���մϴ�.
       * @return	Ǯ ��ũ�� ��� ����
       */
      bool IsFullScreen() const { return m_windowStyle == EWindowStyle::FullScreen || m_windowStyle == EWindowStyle::Borderless; }

      virtual void Update() override;

      void SetTitle(const String& title);

      void _OnWindowResize(unsigned int width, unsigned int height);
      void _OnWindowMinimized();

   private:
      HWND m_handle;
      unsigned int m_resWidth;
      unsigned int m_resHeight;
      Float        m_refreshRate;

      EWindowStyle m_windowStyle;

   public:
      OnWindowResizeMulticastDelegate OnWindowResize;
      OnWindowMinimizedMulticastDelegate OnWindowMinimized;

   };

   LRESULT CALLBACK WinProc(HWND Handle, unsigned int Msg, WPARAM wParam, LPARAM lParam);
}
