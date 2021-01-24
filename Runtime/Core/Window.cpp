#include "Core/Window.h"
#include "Core/Context.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Core/Config.h"
#include "Core/InputManager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace Mile
{
   DEFINE_LOG_CATEGORY(MileWindow);

   Window::Window(Context* context) : SubSystem(context),
      m_handle(nullptr),
      m_resWidth(0), m_resHeight(0),
      m_refreshRate(60.0), m_bIsFullScreen(false)
   {
   }

   Window::~Window()
   {
      DeInit();
   }

   bool Window::Init()
   {
      Context* context = GetContext();
      if (SubSystem::Init())
      {
         auto configSys = context->GetSubSystem<ConfigSystem>();

         auto config = configSys->GetConfig(TEXT("Engine"));
         std::wstring windowTitle = String2WString(config.second["Title"]);

         WNDCLASS wndClass = { 0 };
         wndClass.style = CS_OWNDC;
         wndClass.lpfnWndProc = &Mile::WinProc;
         wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
         wndClass.lpszClassName = windowTitle.c_str();
         RegisterClass(&wndClass);

         m_resWidth = config.second["ResolutionWidth"];
         m_resHeight = config.second["ResolutionHeight"];

         int systemResX = GetSystemMetrics(SM_CXSCREEN);
         int systemResY = GetSystemMetrics(SM_CYSCREEN);

         unsigned int posX = (systemResX - m_resWidth) / 2;
         unsigned int posY = (systemResY - m_resHeight) / 2;

         EWindowStyle windowStyle = IndexToWindowStyle(config.second["WindowStyle"]);
         RECT windowRect;
         windowRect.left = 0;
         windowRect.right = m_resWidth;
         windowRect.bottom = m_resHeight;
         windowRect.top = 0;

         auto style = WS_POPUP;
         auto exStyle = WS_EX_APPWINDOW;
         switch (windowStyle)
         {
         case EWindowStyle::Windowed:
            style = WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
            if (AdjustWindowRectEx(&windowRect, style, false, exStyle) == 0)
            {
               return false;
            }

            break;

         case EWindowStyle::Borderless:
            break;

         case EWindowStyle::FullScreen:
            posX = 0;
            posY = 0;
            windowRect.right = systemResX;
            windowRect.bottom = systemResY;
            WinSetVideoMode(systemResX, systemResY, 32);
            ShowCursor(false);
            break;
         }

         m_handle = CreateWindowEx(
            exStyle,
            windowTitle.c_str(), windowTitle.c_str(),
            style,
            posX, posY,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr, nullptr, nullptr, nullptr);

         ShowWindow(m_handle, SW_SHOW);

         ME_LOG(MileWindow, Log, TEXT("Window Initialized."));
         SubSystem::InitSucceed();
         return true;
      }

      ME_LOG(MileWindow, Warning, TEXT("Window already initialized!"));
      return false;
   }

   void Window::DeInit()
   {
      if (IsInitialized())
      {
         DestroyWindow(m_handle);
         ME_LOG(MileWindow, Log, TEXT("Window deinitialized."));
         SubSystem::DeInit();
      }
   }

   void Window::Update()
   {
      Context* context = GetContext();
      MSG msg = { 0 };
      if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
      {
         auto message = msg.message;
         if (message == WM_QUIT || message == WM_CLOSE || message == WM_DESTROY)
         {
            // Dummy flag for shutting down after 1 frame
            context->GetSubSystem<Engine>()->ShutDownFlagEnable();
         }

         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   void Window::SetTitle(const String& title)
   {
      SetWindowText(m_handle, title.c_str());
   }

   LRESULT WinProc(HWND Handle, unsigned int Msg, WPARAM wParam, LPARAM lParam)
   {
      if (ImGui_ImplWin32_WndProcHandler(Handle, Msg, wParam, lParam))
      {
         return true;
      }

      switch (Msg)
      {
      case WM_DESTROY:
      case WM_CLOSE:
         PostQuitMessage(0);
         return 0;
      }

      InputManager* inputManager = Engine::GetInputManager();
      if (inputManager != nullptr)
      {
         inputManager->HandleWin32(Msg, wParam, lParam);
      }

      return DefWindowProc(Handle, Msg, wParam, lParam);
   }
}