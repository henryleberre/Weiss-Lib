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

	Image::Image(const uint16_t width, const uint16_t height, const Coloru8& fillColor)
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
		std::ifstream inputFile(filepath, std::ios::binary | std::ios::ate);
		if (!inputFile.is_open())
			WS_THROW("[WS] Could Not Read/Open Image File");

		// Get Input File Size
		const uint64_t inputFileSize = inputFile.tellg();
		inputFile.seekg(0, std::ios::beg);

		// Read Input File Into Buffer
		uint8_t* inputFileBuffer = new uint8_t[inputFileSize]; // Dynamic #0
		if (!inputFile.read((char*)inputFileBuffer, inputFileSize))
			WS_THROW("[WS] Could Not Read Image File Into Buffer");
		
		// PNG --> Check Header
		const uint64_t standardPngHeader = 0x0A1A0A0D474E5089; // swap_endian(0x89504E470D0A1A0A)
		if (*(uint64_t*)inputFileBuffer != standardPngHeader)
			WS_THROW("[WS] Input Image Did Not Have The Standard PNG Header");
		
		// Variables Used By The Decoder
		size_t  currentInputFileOffset = sizeof(uint64_t);
		uint8_t bitDepth = 0u, colorType = 0u, compressionMethod = 0u, filterMethod = 0u, interlaceMethod = 0u;
		size_t  rawCombinedIdatDataSize = 0u;

		// Prepare A Vector Containing the IDATs' data (Length Of Data, ptr to start of data)
		std::vector<std::pair<size_t, uint8_t*>> idatssLengthAndPtr;

		// PNG --> Read Chunks
		bool bWasLastChunkRead = false;
		do {
			const uint32_t chunk_length = WS::SwapEndian(*(uint32_t*)(inputFileBuffer + currentInputFileOffset + 0u * sizeof(uint32_t)));
			const uint32_t chunk_name   = *(uint32_t*)(inputFileBuffer + currentInputFileOffset + 1u * sizeof(uint32_t));

			switch (chunk_name) {
			case 0x52444849: // IHDR (swap_endian(0x44484952) i.e swap_endian("IHDR"))
				this->m_width  = WS::SwapEndian(*(uint32_t*)(inputFileBuffer + currentInputFileOffset + 2u * sizeof(uint32_t)));
				this->m_height = WS::SwapEndian(*(uint32_t*)(inputFileBuffer + currentInputFileOffset + 3u * sizeof(uint32_t)));

				this->m_nPixels = this->m_width * this->m_height;

				bitDepth          = *(inputFileBuffer + currentInputFileOffset + 4u * sizeof(uint32_t) + 0u);
				colorType         = *(inputFileBuffer + currentInputFileOffset + 4u * sizeof(uint32_t) + 1u);
				compressionMethod = *(inputFileBuffer + currentInputFileOffset + 4u * sizeof(uint32_t) + 2u);
				filterMethod      = *(inputFileBuffer + currentInputFileOffset + 4u * sizeof(uint32_t) + 3u);
				interlaceMethod   = *(inputFileBuffer + currentInputFileOffset + 4u * sizeof(uint32_t) + 4u);

				break;
			case 0x54414449: // IDAT (swap_endian(0x41444954) i.e swap_endian("IDAT"))
				idatssLengthAndPtr.emplace_back(std::make_pair(chunk_length, inputFileBuffer + currentInputFileOffset + 2 * sizeof(uint32_t)));

				rawCombinedIdatDataSize += chunk_length;

				break;
			case 0x444E4549: // IEND (swap_endian(0x49454E44) i.e swap_endian("IEND"))
				bWasLastChunkRead = true;
				break;
			default:
				if (IsCharUppercase(((uint8_t*)&chunk_name)[0]))
					WS_THROW("[WS] An Important Chunk In A PNG File Is Not Supported (Yet)");
			}

			currentInputFileOffset += 12u;
			currentInputFileOffset += chunk_length;

		} while (!bWasLastChunkRead);

		// Merge IDAT Data Into A Single Buffer
		uint8_t* rawCombinedIdatData = new uint8_t[rawCombinedIdatDataSize]; // Dynamic #1
		
		size_t currentOffsetInRawCombinedIdatData = 0u;
		for (auto& lengthPtrPair : idatssLengthAndPtr)
			std::memcpy(rawCombinedIdatData + currentOffsetInRawCombinedIdatData, lengthPtrPair.second, lengthPtrPair.first);
		delete[] inputFileBuffer; // Dynamic #0

		// Uncompress The Combined IDAT Data
		for (size_t i = 0; i < rawCombinedIdatDataSize; i++)
		{
			std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)rawCombinedIdatData[i] << ',';
		}

		delete[] rawCombinedIdatData; // Dynamic #1
    }

    void Image::Write(const char* filepath) WS_NOEXCEPT
    {
		
    }

}; // WS