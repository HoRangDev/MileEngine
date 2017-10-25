#pragma once
#pragma warning( disable : 4251 )

#include <vector>
#include <array>

#include <memory>
#include <functional>

#define MEAPI __declspec(dllexport)

namespace Mile
{
   /**
   * @brief    �־��� ���ڿ��� ��ū�� �������� �����ϴ�.
   * @param    Array�� ����µ� ���� �Ҵ���
   * @param    ���� ���ڿ�
   * @param    ��ū
   * @return   �������� ���ڿ����� �迭
   */
   static std::vector<std::string> SplitStr( const std::string& str, char token )
   {
      std::vector<std::string> tempArr{ };
      size_t before = 0;
      for ( size_t idx = 0; idx < str.length( ); ++idx )
      {
         if ( str[ idx ] == token )
         {
            std::string tempStr = str.substr( before, idx - before );
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
   static std::string CombineStr( std::vector<std::string> strings, const std::string& token = "" )
   {
      std::string temp{ };

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
}