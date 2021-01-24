#pragma once
#include "Core/SubSystem.h"
#include <forward_list>

namespace Mile
{
   /**
    * @brief   �α��� �������� ��Ÿ���ϴ�.
    *  Fatal   Fatal ������ �ΰŰ� ��Ȱ��ȭ �Ǿ��ִ��� �ְܼ� �α� ���Ͽ� ������ ��� �˴ϴ�.
    *  Error   Error ������ �ְܼ� �α� ���Ͽ� ��� �˴ϴ�.
    *  Warning Warning ������ �ְܼ� �α� ���Ͽ� ��� �˴ϴ�.
    *  Display Display ������ �α״� �ְܼ� �α� ���Ͽ� ��� �˴ϴ�.
    *  Log     �α״� �α� ���Ͽ��� ��ϵ����� �ΰ��� �ֿܼ��� ��� ���� �ʽ��ϴ�.
    */
   enum MEAPI ELogVerbosity : UINT32
   {
      All = 0,
      Fatal,
      Error,
      Warning,
      Display,
      Log,
      EnumSize
   };

   struct MEAPI LogCategoryBase
   {
   public:
      LogCategoryBase(const Mile::String& name, ELogVerbosity defaultVerbosity) :
         Name(name),
         DefaultVerbosity(defaultVerbosity)
      {
      }

   public:
      Mile::String Name = TEXT("Unknown");
      ELogVerbosity DefaultVerbosity = ELogVerbosity::Log;

   };

   struct MEAPI MLog
   {
      LogCategoryBase Category;
      ELogVerbosity Verbosity = ELogVerbosity::All;
      Mile::String Message = TEXT("");
      std::chrono::system_clock::time_point Time = std::chrono::system_clock::time_point();
   };

   using LogList = std::forward_list<MLog>;
   class MEAPI Logger : public SubSystem
   {
   public:
      Logger(Context* context);
      virtual ~Logger();

      virtual bool Init() override;
      virtual void DeInit() override;

      void SetLogFolderPath(const Mile::String& folderPath);

      void Logging(const LogCategoryBase& category,
         ELogVerbosity verbosity = ELogVerbosity::All,
         const Mile::String& message = TEXT(""));

      LogList Filtering(const LogCategoryBase& category, ELogVerbosity verbosity);

      bool Flush();
      bool Flush(const LogCategoryBase& category, ELogVerbosity verbosity = ELogVerbosity::All);

      static Mile::String VerbosityToString(ELogVerbosity verbosity)
      {
         switch (verbosity)
         {
         default:
         case ELogVerbosity::All:
            return TEXT("All");
         case ELogVerbosity::Fatal:
            return TEXT("Fatal");
         case ELogVerbosity::Error:
            return TEXT("Error");
         case ELogVerbosity::Log:
            return TEXT("Log");
         case ELogVerbosity::Warning:
            return TEXT("Warning");
         }
      }

      static int VerbosityToConsoleColor(ELogVerbosity type)
      {
         switch (type)
         {
         default:
            return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
         case ELogVerbosity::Log:
            return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
         case ELogVerbosity::Warning:
            return FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
         case ELogVerbosity::Fatal:
         case ELogVerbosity::Error:
            return FOREGROUND_RED | FOREGROUND_INTENSITY;
         }
      }

      static Mile::String LogToStr(const MLog& log)
      {
         // [Category][Type][Time] Message
         auto typeStr = Logger::VerbosityToString(log.Verbosity);
         auto timeStr = TimeToWString(log.Time);
         auto result = Formatting(TEXT("[%s][%s][%s] %s"),
            timeStr.c_str(),
            log.Category.Name.c_str(),
            typeStr.c_str(),
            log.Message.c_str());
         return result;
      }

      static void GlobalLogging(const LogCategoryBase& category,
         ELogVerbosity verbosity = ELogVerbosity::All,
         const Mile::String& message = TEXT(""));

   private:
      bool Flush(const LogList& list);

   private:
      Mile::String   m_folderPath;
      LogList        m_logs;
      std::chrono::system_clock::time_point     m_loggingBeginTime;

   };

#define DECLARE_LOG_CATEGORY_EXTERN(CategoryName, DefaultVerbosity) \
   namespace DefinedLogCategoryType { \
      struct CategoryName##Type : public LogCategoryBase { \
      public: \
         CategoryName##Type() : LogCategoryBase(TEXT(#CategoryName), DefaultVerbosity) \
         { \
         } \
      }; \
   } \
   extern DefinedLogCategoryType::CategoryName##Type CategoryName \

#define DEFINE_LOG_CATEGORY(CategoryName) DefinedLogCategoryType::CategoryName##Type CategoryName

/* Mile Engine Log **/
#define ME_LOG(CategoryName, Verbosity, Message) Logger::GlobalLogging(CategoryName, Verbosity, Message)

   DECLARE_LOG_CATEGORY_EXTERN(MileLogger, Log);
}