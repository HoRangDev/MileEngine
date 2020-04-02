#pragma once
#include "Resource/Resource.h"

namespace Mile
{
   class Texture2dDX11;
   class MEAPI Texture2D : public Resource
   {
   public:
      Texture2D(Context* context, const String& filePath);
      virtual ~Texture2D();

      virtual bool Init() override;
      virtual bool Save(const String& filePath);

      unsigned char* GetRawData() const { return m_rawData; }

      bool HasRawTexture() const { return m_rawTexture != nullptr; }
      bool InitRawTexture();
      Texture2dDX11* GetRawTexture() const { return m_rawTexture; }

   private:
      unsigned char* m_rawData;
      Texture2dDX11* m_rawTexture;

      unsigned int   m_width;
      unsigned int   m_height;
      int            m_channels;
      unsigned int   m_bitDepth;
      unsigned int   m_bitPerChannel;
      bool           m_bIsHDR;

   };
}
