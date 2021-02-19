#pragma once
#pragma warning(disable : 4251)
#pragma warning(disable : 4996)
#pragma warning(default : 4265)
#pragma warning(push, 0)
#include <vector>
#include <array>
#include <map>
#include <set>
#include <forward_list>
#include <queue>
#include <tuple>
#include <stack>
#include <memory>
#include <functional>
#include <locale>
#include <codecvt>
#include <iostream>
#include <string>
#include <algorithm>
#include <type_traits>
#include <thread>
#include <future>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <chrono>
#include <random>
#include <iterator>
#include <atomic>
#include <cstdio>
#include <fstream>
#include <filesystem>

#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

#include <json.hpp>
using json = nlohmann::json;

#include <optick.h>

#include <FreeImage.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#pragma warning(pop)

#ifdef RUNTIME_EXPORTS
#define MEAPI __declspec(dllexport)
#else
#define MEAPI __declspec(dllimport) 
#endif 

#define NULL_TEXT TEXT("null")
#define NULL_TEXT_STD "null"

namespace Mile
{
   using String = std::wstring;
   using Float = float;

   template <typename... Args>
   String Format(const String& format, Args... args)
   {
      int size = _snwprintf(nullptr, 0, format.c_str(), args...) + 1;
      if (size <= 0)
      {
         throw std::runtime_error("Error during formatting!");
      }

      std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
      _snwprintf(buf.get(), size, format.c_str(), args...);
      return String(buf.get(), buf.get() + size - 1);
   }

   static std::string BoolSerialize(bool value)
   {
      if (value)
      {
         return "true";
      }

      return "false";
   }

   static bool StringToBool(const std::string& value)
   {
      if (value == "true")
      {
         return true;
      }

      return false;
   }

   /**
   * @brief    �־��� ���ڿ��� ��ū�� �������� �����ϴ�.
   * @param    Array�� ����µ� ���� �Ҵ���
   * @param    ���� ���ڿ�
   * @param    ��ū
   * @return   �������� ���ڿ����� �迭
   */
   static std::vector<String> SplitStr(const String& str, char token)
   {
      std::vector<String> tempArr{ };
      String tempStr{ };

      size_t length = str.length();

      for (size_t idx = 0; idx < length; ++idx)
      {
         if (str[idx] != token)
         {
            tempStr += str[idx];
         }

         if (str[idx] == token || idx == (length - 1))
         {
            tempArr.push_back(tempStr);
            tempStr.clear();
         }
      }

      return tempArr;
   }

   /**
   * @brief    �־��� ���ڿ����� �迭�� ��Ĩ�ϴ�.
   * @param    ��ĥ ���ڿ����� �迭
   * @param    ���ڿ����� ��ġ�鼭 �߰��� �� ��ū
   * @return   ������ ���ڿ�
   */
   static String CombineStr(std::vector<String> strings, const String& token = TEXT(""))
   {
      String temp{ };

      for (const auto& str : strings)
      {
         temp += str;
         temp += token;
      }

      return temp;
   }

   /**
   * @brief  Release�� ���� �޸� �Ҵ� ������ �ϴ� �ν��Ͻ��� �����ϰ� ���������ݴϴ�.
   */
   template <typename T>
   void SafeRelease(T& target)
   {
      if (target != nullptr)
      {
         target->Release();
         target = nullptr;
      }
   }

   /**
   * @brief Delete�� ���� �޸𸮸� �Ҵ� ���� �ؾ��ϴ� �ν��Ͻ��� �����ϰ� ���������ݴϴ�.
   */
   template <typename T>
   void SafeDelete(T& target)
   {
      if (target != nullptr)
      {
         delete target;
         target = nullptr;
      }
   }

   template <typename T>
   void SafeArrayDelete(T& target)
   {
      if (target != nullptr)
      {
         delete[] target;
         target = nullptr;
      }
   }

   template <typename T>
   void SafeFree(T& target)
   {
      if (target != nullptr)
      {
         std::free(target);
         target = nullptr;
      }
   }

   static String String2WString(const std::string& str)
   {
      static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      if (str.empty())
      {
         return std::wstring();
      }

      return converter.from_bytes(str);
   }

   static std::string WString2String(const String& str)
   {
      static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      if (str.empty())
      {
         return std::string();
      }

      return converter.to_bytes(str);
   }

   template <typename ... Args>
   std::string Formatting(const std::string& str, Args ... args)
   {
      int size = snprintf(nullptr, 0, str.c_str(), args ...) + 1;
      std::unique_ptr< char[] > buf(new char[size]);
      snprintf(buf.get(), size, str.c_str(), args ...);
      return std::string(buf.get(), buf.get() + size - 1);
   }

   template < typename ... Args>
   String Formatting(const String& str, Args ... args)
   {
      int size = _snwprintf(nullptr, 0, str.c_str(), args ...) + 1;
      std::unique_ptr< wchar_t > buf(new wchar_t[size]);
      _snwprintf(buf.get(), size, str.c_str(), args ...);
      return String(buf.get(), buf.get() + size - 1);
   }

   /*
   * @param format   Require 6 integer formatting character( %d )
   *                 "%d/%d/%d %d:%d:%d" => "Year/Month/Day Hour:Min:Sec"
   **/
   static std::string TimeToString(const std::chrono::system_clock::time_point& t,
      const std::string& format = "%d/%02d/%02d %02d:%02d:%02d")
   {
      using namespace std;
      time_t rawTime = chrono::system_clock::to_time_t(t);

      tm timeInfo{};
      localtime_s(&timeInfo, &rawTime);
      return Formatting(format,
         timeInfo.tm_year + 1900,
         timeInfo.tm_mon + 1,
         timeInfo.tm_mday,
         timeInfo.tm_hour,
         timeInfo.tm_min,
         timeInfo.tm_sec);
   }

   /*
   * @param format   Require 6 integer formatting character( %d )
   *                 "%d/%d/%d %d:%d:%d" => "Year/Month/Day Hour:Min:Sec"
   **/
   static String TimeToWString(const std::chrono::system_clock::time_point& t,
      const String& format = TEXT("%d/%02d/%02d %02d:%02d:%02d"))
   {
      using namespace std;
      time_t rawTime = chrono::system_clock::to_time_t(t);

      tm timeInfo{};
      localtime_s(&timeInfo, &rawTime);
      return Formatting(format,
         timeInfo.tm_year + 1900,
         timeInfo.tm_mon + 1,
         timeInfo.tm_mday,
         timeInfo.tm_hour,
         timeInfo.tm_min,
         timeInfo.tm_sec);
   }

   static std::string NowToString()
   {
      using namespace std;
      chrono::system_clock::time_point t = chrono::system_clock::now();
      return TimeToString(t);
   }

   static String NowToWString()
   {
      using namespace std;
      chrono::system_clock::time_point t = chrono::system_clock::now();
      return TimeToWString(t);
   }

   static String ToLower(const String& target)
   {
      String temp = target;
      std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
      return temp;
   }

   template <typename Ty>
   Ty GetValueSafelyFromJson(const json& container, const std::string& key, const Ty& defaultValue = Ty())
   {
      auto foundValue = container.find(key);
      return (foundValue != container.end()) ? (Ty)(*foundValue) : defaultValue;
   }
}