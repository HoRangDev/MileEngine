#include "Win32Window.h"

namespace Mile
{
    bool Win32Window::Initialize( )
    {
        std::wstring Title = static_cast<std::wstring>( GetTitle( ) );

        /* ���⼭ Win32 �ʱ�ȭ */
        WNDCLASS WinClass = { NULL };
        WinClass.style = CS_OWNDC;
        //WinClass.lpfnWndProc = ...;

        /* ���߿� DirectX ���� Ŀ���� Ŀ�� �����ϱ� */
        WinClass.hCursor = LoadCursor( nullptr, IDC_ARROW );
        WinClass.lpszClassName = Title.c_str( );
        RegisterClass( &WinClass );

        /* ȭ���忡 ���� ������ ó���ϱ� */
        uint64 Width = GetWidth( );
        uint64 Height = GetHeight( );
        Handle = CreateWindow(
            Title.c_str( ), Title.c_str( ),
            WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
            0, 0, Width, Height, /* @todo: ������ġ ���Ҽ� �ֵ��� �ϱ� */
            nullptr, nullptr, nullptr, nullptr );

        if ( Handle == nullptr )
        {
            ASSERT_MSG( (true), TEXT( "Win32Window failed to initialize." ) )
            return false;
        }

        return true;
    }

    void Win32Window::DeInitialize( )
    {
        /* @todo: �Ҵ� ���� �۾� */
    }

    void Win32Window::OnResize( uint64 NewWidth, uint64 NewHeight )
    {
        /* @todo: ������ �޼��� �ݹ� �� �ʿ��� ����� �߰������� ó��. */
    }

    void Win32Window::OnWindowModeChange( EWindowMode NewWindowMode )
    {
        /* @todo: ������ ��尡 ���� �߰����� �۾� */
    }
}