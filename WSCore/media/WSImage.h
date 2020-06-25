#pragma once

#include "../misc/WSPch.h"
#include "../math/WSVector.h"
#include "../misc/WSBitLogic.h"

#define WS_PNG_IHDR_CHUNK_NAME_RAW 0x49484452
#define WS_PNG_IDAT_CHUNK_NAME_RAW 0x49444154
#define WS_PNG_IEND_CHUNK_NAME_RAW 0x49454E44

namespace WS {

	/*
	 * The "Image" class represents a 4 byte per pixel image (r, g, b, a)
	 */
	class Image {
	private:
		std::unique_ptr<WS::Coloru8[]> m_pBuff;

		uint32_t m_width   = 0, m_height = 0;
		uint64_t m_nPixels = 0;

	public:
		Image() = default;

		Image(Image&& other) noexcept;
		Image(const Image& other) noexcept;

		Image(const char* filepath) WS_NOEXCEPT;

		Image(const uint32_t width, const uint32_t height, const Coloru8& fillColor = { 0, 0, 0, 255 });

		[[nodiscard]] inline uint32_t GetWidth()      const noexcept { return this->m_width;   }
		[[nodiscard]] inline uint32_t GetHeight()     const noexcept { return this->m_height;  }
		[[nodiscard]] inline uint64_t GetPixelCount() const noexcept { return this->m_nPixels; }

		inline void SetPixelColor(const uint32_t x, const uint32_t y, const WS::Coloru8& color) WS_NOEXCEPT
		{
#ifdef __WEISS__DEBUG_MODE
		
			assert(x < this->m_width && y < this->m_height);
		
#endif // __WEISS__DEBUG_MODE

			this->m_pBuff[y * this->m_width + x] = color;
		}

		[[nodiscard]] inline WS::Coloru8 SamplePixelColor(const uint32_t x, const uint32_t y) WS_NOEXCEPT
		{
#ifdef __WEISS__DEBUG_MODE
		
			assert(x < this->m_width && y < this->m_height);
		
#endif // __WEISS__DEBUG_MODE

			return this->m_pBuff[y * this->m_width + x];
		}

		void Write(const char* filepath) WS_NOEXCEPT;
	};

};