#include "Timer.h"
#include "Logger.h"

namespace Mile
{
   Timer::Timer( Context* context ) : 
      m_frameCount( 0 ), m_framePerSec( 0 ),
      SubSystem( context )
   {
   }

   Timer::~Timer( )
   {
   }

   bool Timer::Init( )
   {
      m_frameBeginTime = m_frameEndTime = std::chrono::steady_clock::now( );
      m_deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>
         ( m_frameEndTime - m_frameBeginTime );
      m_frameCount = 0;
      MELog( m_context, TEXT( "Timer" ), ELogType::MESSAGE, TEXT( "Timer initialized." ), true );
      return true;
   }

   void Timer::Update( )
   {
      if ( m_frameCount == 0 )
      {
         m_frameMeasureBeginTime = std::chrono::steady_clock::now( );
      }

      ++m_frameCount;

      auto now = std::chrono::steady_clock::now( );
      auto dt = now - m_frameMeasureBeginTime;
      auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( dt );

      // 1000 ms = 1 sec
      if ( duration.count( ) > 1000 )
      {
         m_framePerSec = m_frameCount;
         m_frameCount = 0;
      }
   }

   void Timer::BeginFrame( )
   {
      m_frameBeginTime = std::chrono::steady_clock::now( );
   }

   void Timer::EndFrame( )
   {
      m_frameEndTime = std::chrono::steady_clock::now( );
      m_deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(m_frameEndTime - m_frameBeginTime);
   }
}