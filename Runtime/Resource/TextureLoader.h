#pragma once

#include <tuple>
#include "Core\Helper.h"

namespace Mile
{
	/**
	 * @brief	�ؽ��Ŀ� ���� ������ ����ִ� Ʃ�� ��ü �Դϴ�. <RawData, Width, Height, Channels>
	 */
   using TextureInfo = std::tuple<unsigned char*, unsigned int, unsigned int, int>;
   enum TextureInfoTag
   {
		RAWDATA = 0,
		WIDTH = 1,
		HEIGHT = 2,
		CHANNELS = 3,
   };

   class MEAPI TextureLoader
   {
   public:
      static TextureInfo LoadTexture( const String& inFilePath, const String& fileExtension );
   };
}