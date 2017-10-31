#pragma once
#pragma warning( disable : 4251 )

#include <vector>
#include <array>

#include <memory>
#include <functional>

#include <locale>
#include <codecvt>
#include <string>

#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

#define MEAPI __declspec(dllexport)

namespace Mile
{
   using String = std::wstring;

#ifdef _FLOAT_AS_DOUBLE_
   using Float = double;
#else
   using Float = float;
#endif


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
      size_t before = 0;
      for ( size_t idx = 0; idx < str.length( ); ++idx )
      {
         if ( str[ idx ] == token )
         {
            String tempStr = str.substr( before, idx - before );
            if ( !tempStr.empty( ) )
            {
               tempArr.push_back( std::move( tempStr ) );
            }
            before = idx;
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

   static std::wstring String2WString( const std::string& str )
   {
      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      return converter.from_bytes( str );
   }

   static std::string WString2String( const std::wstring& str )
   {
      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      return converter.to_bytes( str );
   }
}