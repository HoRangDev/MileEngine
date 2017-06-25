#pragma once
#include "MileObject.h"

namespace Mile
{
    /**
    * Ÿ�̸� Ŭ�����Դϴ�.
    */
    class MILE_API Timer final : public Mile::Object
    {
    public:
        ~Timer( );

        /**
        * Ÿ�̸Ӹ� Tick ��ŵ�ϴ�.
        */
        void Tick( );

        /**
        * Ÿ�̸Ӹ� �ʱ�ȭ ��ŵ�ϴ�.
        */
        void Reset( );

        /**
        * Ÿ�̸Ӱ� ���� �ð��� ��ȯ�մϴ�.
        * @return Ÿ�̸��� ���� �ð�
        */
        float GetRuntime( ) const;

        /**
        * Ÿ�̸��� ��Ÿ �ð��� ��ȯ�մϴ�. (Tick �޼ҵ� ȣ�� ������ �ð���)
        * @param ��Ÿ �ð�
        */
        float GetDeltaTime( ) const;

        /**
        * Ÿ�̸Ӱ� ī��Ʈ�� �����Ӽ��� ��ȯ�մϴ�.
        * @return ������ ��
        */
        uint64 GetFrameCount( ) const;

        /**
        * Ÿ�̸Ӱ� ���������� ������ �ʴ� �����Ӽ��� ��ȯ�մϴ�.
        * @return �ʴ� ������ ��
        */
        uint64 GetFPS( )  const;

        /**
        * Ÿ�̸Ӱ� ������ �ʴ� �����Ӽ�ġ�� ���� ���� ��ġ�� ��ȯ�մϴ�.
        * @return ������ ��ġ�� ����ū �ʴ� ������ ��
        */
        uint64 GetMaxFPS( ) const;

        /**
        * ������ Scale�� �������� Delta time �� ��ȯ�մϴ�.
        * @return Scaled Delta time
        */
        float GetScaledDeltaTime( ) const;

        /**
        * ������ Time scale�� ��ȯ�մϴ�.
        * @return Time scale value
        */
        float GetTimeScale( ) const;

        /**
        * Time Scale�� �����մϴ�.
        * @param Scale ������ Scale ��
        */
        void SetTimeScale( float Scale );

    private:
        /**
        * Ÿ�̸� Ŭ������ �⺻ ������
        * @param NewName Ÿ�̸��� ������Ʈ �̸�
        */
        Timer( Rumia::Allocator& Allocator, const class MString& NewName );

    private:
        uint64 TicksPerSec;

        uint64 StartupTicks;
        uint64 LastTicks;
        uint64 CurrentTicks;
        uint64 OneSecElasedTicks;

        float DeltaTime;
        uint64 FrameCount;
        uint64 FPS;
        uint64 MaxFPS;

        float TimeScale;

    };
}