#pragma once
#include "MileObject.h"
#include "MileString.h"

namespace Mile
{
    enum class EWindowMode
    {
        Windowed,
        FullWindowed,
        FullScreen
    };

    /**
    * â�� ��Ÿ���� Ŭ����
    */
    class MILE_API Window : public Mile::Object
    {
    public:

        /**
        * �������� ������
        * @param NewTitle ������ �������� Ÿ��Ʋ
        * @param NewWidth ������ �������� �ʺ�
        * @param NewHeight ������ �������� ����
        * @param NewWindowMode ������ ������ ���
        */
        Window( const MString& NewTitle,
            uint32 NewWidth,
            uint32 NewHeight,
            EWindowMode NewWindowMode ) :
            Width( NewWidth ), Height( NewHeight ),
            WindowMode( NewWindowMode ),
            Object( NewTitle )
        {
        }

        /**
        * �������� �Ҹ���
        */
        virtual ~Window( ) {}

        /**
        * �������� Ÿ��Ʋ�� �����ɴϴ�.
        */
        MString GetTitle( ) const { return GetName( ); }

        /**
        * �������� ���� �ʺ� �����ɴϴ�.
        * @return �������� ���� �ʺ�.
        */
        FORCEINLINE uint32 GetWidth( ) const { return Width; }
        /**
        * �������� ���� ���̸� �����ɴϴ�.
        * @return �������� ���� ����.
        */
        FORCEINLINE uint32 GetHeight( ) const { return Height; }

        /**
        * �������� ���� ������ ��带 �����ɴϴ�.
        * @return �������� ���� �������� ���
        */
        FORCEINLINE EWindowMode GetWindowMode( ) const { return WindowMode; }

        /**
        * �����츦 �������� �ϴ� �޼ҵ��Դϴ�.
        * @param NewWidth ���� ������ �ʺ�
        * @param NewHeight ���� ������ ����
        */
        void Resize( uint32 NewWidth, uint32 NewHeight );

        /**
        * �������� ������ ��带 �����մϴ�.
        * @param NewWindowMode ���� ������ ������ ���
        */
        void SetWindowMode( EWindowMode NewWindowMode );

        virtual bool Initialize( ) = 0;
        virtual void DeInitialize( ) = 0;

        /**
        * �������� ũ�Ⱑ �缳���Ǿ��� �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
        * @param NewWidth ���� ������ �ʺ�
        * @param NewHeight ���� ������ ����
        */
        virtual void OnResize( uint32 NewWidth, uint32 NewHeight ) = 0;

        /**
        * �������� ������ ��尡 �缳���Ǿ��� �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
        * @param NewWindowMode ���� ������ ���
        */
        virtual void OnWindowModeChange( EWindowMode NewWindowMode ) = 0;

    private:
        uint32 Width;
        uint32 Height;

        EWindowMode WindowMode;

    };
}