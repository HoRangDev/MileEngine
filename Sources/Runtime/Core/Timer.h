#pragma once
#include "Core/Logger.h"

namespace Mile
{
   DECLARE_LOG_CATEGORY_EXTERN(MileTimer, Log);

   /**
    * @brief	Timer ���� �ý����� ���� ������ �ð��� ���õ� ������ �����ϰų� �������ִ� Ŭ���� �Դϴ�.
    */
   class MEAPI Timer : public SubSystem
   {
   public:
      Timer(Context* context);
      virtual ~Timer();

      virtual bool Init() override;
      virtual void DeInit() override;

      void BeginFrame();
      /**
       * @brief   ���� �ð���ŭ CPU �� sleep �ϱ� ������ ȣ��
       */
      void PreEndFrame();

      /**
       * @brief   CPU�� sleep���� ������� ȣ��(������ �����Ӵ� �ð����� ������ ó���� �� ���� �������� delta time ���� �ٽ� ���)
       */
      void PostEndFrame();

      /**
       * @brief	���� �����Ӱ� ���� �����ӻ��̿� �ð� ���̸� '��' ������ ��ȯ ���ݴϴ�.
       * @return �����Ӱ� ���� �ð�(��)
       */
      float GetDeltaTime() const
      {
         return GetDeltaTimeNS() * 0.000000001f;
      }

      /**
      * @brief	���� �����Ӱ� ���� �����ӻ��̿� �ð� ���̸� 'ms' ������ ��ȯ ���ݴϴ�.
      * @return �����Ӱ� ���� �ð�(ms)
      */
      double GetDeltaTimeMS() const
      {
         return (GetDeltaTimeNS() * 0.000001);
      }

      long long GetDeltaTimeNS() const
      {
         return ((m_deltaTime).count());
      }

      /**
       * @brief	������ �ʴ� ��� �������� �����ߴ��� ��ȯ�մϴ�.
       * @return �ʴ� ������ ��
       */
      long long GetFPS() const
      {
         return m_framePerSec;
      }

   private:
      std::chrono::steady_clock::time_point m_frameBeginTime;
      std::chrono::steady_clock::time_point m_frameEndTime;
      std::chrono::nanoseconds m_deltaTime;

      std::chrono::steady_clock::time_point m_frameMeasureBeginTime;
      long long   m_frameCount;
      long long   m_framePerSec;

   };
}