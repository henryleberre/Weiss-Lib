#include "WSImage.h"

namespace WS {

	Image::Image(Image&& other) noexcept
		: m_width(other.m_width), m_height(other.m_height), m_nPixels(other.m_nPixels)
	{
		this->m_pBuff = std::move(other.m_pBuff);
	}

	Image::Image(const Image& other) noexcept
		: m_width(other.m_width), m_height(other.m_height), m_nPixels(other.m_nPixels)
	{
		size_t bufferSize = this->m_nPixels * sizeof(WS::Coloru8);

		this->m_pBuff = std::make_unique<WS::Coloru8[]>(bufferSize);
		std::memcpy(this->m_pBuff.get(), other.m_pBuff.get(), bufferSize);
	}

	Image::Image(const uint32_t width, const uint32_t height, const Coloru8& fillColor)
		: m_width(width), m_height(height), m_nPixels(width * height)
	{
#ifdef __WEISS__DEBUG_MODE

		assert(width != 0 && height != 0);

#endif // __WEISS__DEBUG_MODE

		size_t bufferSize = this->m_nPixels * sizeof(WS::Coloru8);
		this->m_pBuff = std::make_unique<WS::Coloru8[]>(bufferSize);
		std::fill_n(this->m_pBuff.get(), this->m_nPixels, WS::Coloru8{0u, 0u, 0u, 255u});
	}

	Image::Image(const char* filepath) WS_NOEXCEPT
	{
		// Step #0: Read Input File Into Buffer
		uint8_t* fileBuffer; // Dynamic #0
		uint8_t* filePosition;
		{
			std::ifstream file(filepath, std::ios::binary | std::ios::ate);
			if (!file.is_open())
				WS_THROW("[WS] Could Not Read/Open Image File");

			// Create the buffer
			const size_t fileSize = file.tellg();
			fileBuffer = new uint8_t[fileSize];

			// Read File Into The Buffer
			file.seekg(0, std::ios::beg);

			if (!file.read((char*)fileBuffer, fileSize))
				WS_THROW("[WS] Could Not Read Image File Into Buffer");

			filePosition = fileBuffer;
		}
		
		{ // Step #1: Check PNG Header
			const uint64_t standardPngHeader = 0x0A1A0A0D474E5089; // swap_endian(0x89504E470D0A1A0A)
			if (*(uint64_t*)fileBuffer != standardPngHeader)
				WS_THROW("[WS] Input Image Did Not Have The Standard PNG Header");
			
			filePosition += sizeof(standardPngHeader);
		}
		
		struct {
			uint8_t bitDepth = 0u, colorType = 0u, compressionMethod = 0u, filterMethod = 0u, interlaceMethod = 0u;
			size_t rawCombinedIdatDataSize = 0u;
		} pngChunkData;
		{ // Step #2: Read PNG Chunks
			bool bLastChunk = false;

			do {
				 // Read Chunk Metadata
				const uint32_t chunkDataLength = WS::FromBigEndian(*(uint32_t*)(filePosition));
				const uint32_t chunkName       = WS::FromLittleEndian(*(uint32_t*)(filePosition + 4u));

				// Parse Chunks
				switch (chunkName) {
				case WS_PNG_IHDR_CHUNK_NAME_RAW:
					this->m_width  = WS::FromBigEndian(*(uint32_t*)(filePosition + 8u));
					this->m_height = WS::FromBigEndian(*(uint32_t*)(filePosition + 12u));

					std::cout << this->m_width << " " << this->m_height << '\n';
					break;
				case WS_PNG_IDAT_CHUNK_NAME_RAW:
					break;
				case WS_PNG_IEND_CHUNK_NAME_RAW:
					bLastChunk = true;
					break;
				}

				/* 12u is the size in bytes of the fixed sized part of any png chunk header
				 * chunkDataLength is the size in bytes of the dynamic sized part of any png chunk header */
				filePosition += 12u + chunkDataLength;
			} while (!bLastChunk);
		}

		delete[] fileBuffer; // Dynamic #0
    }

    void Image::Write(const char* filepath) WS_NOEXCEPT
    {
		
    }

}; // WS