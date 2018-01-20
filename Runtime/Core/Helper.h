#pragma once
#pragma warning( disable : 4251 )

#include <vector>
#include <array>
#include <map>
#include <set>

#include <memory>
#include <functional>

#include <locale>
#include <codecvt>
#include <string>

#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

#define MEAPI __declspec(dllexport)

#include "json.hpp"
using json = nlohmann::json;

namespace Mile
{
   using String = std::wstring;
   using Float = float;

   static std::string BoolSerialize( bool value )
   {
      if ( value )
      {
         return "true";
      }
      
      return "false";
   }

   /**
   * @brief    �־��� ���ڿ��� ��ū�� �������� �����ϴ�.
   * @param    Array�� ����µ� ���� �Ҵ���
   * @param    ���� ���ڿ�
   * @param    ��ū
   * @return   �������� ���ڿ����� �迭
   */
   static std::vector<String> SplitStr( const String& str, char token )
   {
      std::vector<String> tempArr{ };
      String tempStr{ };

      size_t length = str.length( );

      for ( size_t idx = 0; idx < length; ++idx )
      {
         if ( str[ idx ] != token )
         {
            tempStr += str[ idx ];
         }

         if ( str[ idx ] == token || idx == ( length - 1 ) )
         {
            tempArr.push_back( tempStr );
            tempStr.clear( );
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
   static String CombineStr( std::vector<String> strings, const String& token = TEXT("") )
   {
      String temp{ };

      for ( auto str : strings )
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
   void SafeRelease( T& target )
   {
      if ( target != nullptr )
      {
         target->Release( );
         target = nullptr;
      }
   }

   /**
   * @brief Delete�� ���� �޸𸮸� �Ҵ� ���� �ؾ��ϴ� �ν��Ͻ��� �����ϰ� ���������ݴϴ�.
   */
   template <typename T>
   void SafeDelete( T& target )
   {
      if ( target != nullptr )
      {
         delete target;
         target = nullptr;
      }
   }

   template <typename T>
   void SafeArrayDelete( T& target )
   {
      if ( target != nullptr )
      {
         delete[ ] target;
         target = nullptr;
      }
   }

   template <typename T>
   void SafeFree( T& target )
   {
      if ( target != nullptr )
      {
         std::free( target );
         target = nullptr;
      }
   }

   static String String2WString( const std::string& str )
   {
      if ( str.empty( ) )
      {
         return TEXT( "" );
      }
      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      return converter.from_bytes( str );
   }

   static std::string WString2String( const String& str )
   {
      if ( str.empty( ) )
      {
         return std::string( );
      }

      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      return converter.to_bytes( str );
   }
}