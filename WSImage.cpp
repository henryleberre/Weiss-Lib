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

				if (filterMethod != 0)
					WS_THROW("[WS] Your PNG Filter Method Is Not Supported");

				std::cout << "Width: " << (int)this->m_width << '\n';
				std::cout << "Filter: " << (int)filterMethod << '\n';
				std::cout << "Color Type: " << (int)colorType << '\n';
				std::cout << "Bit Depth: " << (int)bitDepth << '\n';

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

			//TODO:: CRC Check

		} while (!bWasLastChunkRead);

		// Merge IDAT Data Into A Single Buffer
		uint8_t* rawCombinedIdatData = new uint8_t[rawCombinedIdatDataSize]; // Dynamic #1
		
		size_t currentOffsetInRawCombinedIdatData = 0u;
		for (auto& lengthPtrPair : idatssLengthAndPtr) {
			std::memcpy(rawCombinedIdatData + currentOffsetInRawCombinedIdatData, lengthPtrPair.second, lengthPtrPair.first);
			currentOffsetInRawCombinedIdatData += lengthPtrPair.first;
		}
		delete[] inputFileBuffer; // Dynamic #0

		// Uncompress The Combined IDAT Data
		// ZLIB SPEC: https://tools.ietf.org/html/rfc1950

		const uint8_t zlibCMF = *(rawCombinedIdatData + 0u * sizeof(uint8_t));
		const uint8_t zlibFLG = *(rawCombinedIdatData + 1u * sizeof(uint8_t));

		const uint8_t zlibCM    = (zlibCMF & 0b00001111) >> 0u;
		const uint8_t zlibCINFO = (zlibCMF & 0b11110000) >> 4u;

		if (zlibCM != 8)
			WS_THROW("[WS] zlibCM != 8 Is Not (Yet) Supported!");

		const uint32_t zlibWindowSize = (zlibCINFO == 7u) ? std::pow(2, 5) : std::pow(2, zlibCINFO) + 8u;

		const uint8_t zlibFCHECK = (zlibFLG & 0b00011111) >> 0u;
		const bool    zlibFDICT  = (zlibFLG & 0b00100000) >> 5u;
		const uint8_t zlibFLEVEL = (zlibFLG & 0b11000000) >> 6u;

		//TODO:: zlibFCHECK

		if (zlibFDICT) // TODO:: support zlibFDICT
			WS_THROW("[WS] Preset Dicts are not yet supported");

		uint8_t* zlibRawCompressedData       = rawCombinedIdatData + 2u * sizeof(uint8_t);
		uint32_t zlibRawCompressedDataOffset = 0u;

		// Deflate/Inflate Spec: https://tools.ietf.org/html/rfc1951
		bool inflateBFINAL = false;
		std::vector<std::pair<uint16_t, uint8_t*>> inflateUncompressedDataPtrs;
		inflateUncompressedDataPtrs.reserve(10);
		uint32_t totalInflateUncompressedDataLength = 0u;

		do {
			const uint8_t inflateBlockHeader = *(zlibRawCompressedData + zlibRawCompressedDataOffset);

			inflateBFINAL = (inflateBlockHeader & 0b10000000) >> 7u;
			const uint8_t inflateBTYPE = (inflateBlockHeader & 0b01100000) >> 5u;

			switch (inflateBTYPE) {
			case 0b00: // No Compression
			{
				// Read LEN & NLEN After Skipping To The Next Byte Boundary
				const uint16_t inflateLEN  = *(uint16_t*)(zlibRawCompressedData + zlibRawCompressedDataOffset + 1u * sizeof(uint8_t) + 0u * sizeof(uint16_t));
				const uint16_t inflateNLEN = *(uint16_t*)(zlibRawCompressedData + zlibRawCompressedDataOffset + 1u * sizeof(uint8_t) + 1u * sizeof(uint16_t));

				// One's complement Check
				const uint8_t temp = ~inflateNLEN;
				if (temp != inflateLEN)
					WS_THROW("[WS] One's complement Check On LEN & NLEN Failed!");

				const auto& pair = std::make_pair(inflateLEN, (uint8_t*)zlibRawCompressedDataOffset + 1u * sizeof(uint8_t) + 2u * sizeof(uint16_t));
				inflateUncompressedDataPtrs.push_back(pair);
				
				zlibRawCompressedDataOffset        += 1u * sizeof(uint8_t) + 2u * sizeof(uint16_t) + inflateLEN;
				totalInflateUncompressedDataLength += inflateLEN;
				break;
			}
			case 0b01: // Compressed W/ Fixed Huffman Codes
				// TODO:: IMPLEMENT
				break;
			case 0b10: // Compressed W/ Dynamic Huffman Codes
				// TODO:: IMPLEMENT
				break;
			case 0b11: // Reserved
				WS_THROW("[WS] Inflate Chunk Header BTYPE is Reversed (0b11)");
				break;
			}

		} while (!inflateBFINAL);
	
		// Combine Uncompressed Data
		uint8_t* uncompressedIdatData = new uint8_t[totalInflateUncompressedDataLength]; // Dynamic #2
		{
			size_t offset = 0u;
			for (auto& lenAndPtr : inflateUncompressedDataPtrs) {
				std::memcpy(uncompressedIdatData + offset, lenAndPtr.second, lenAndPtr.first);
				offset += lenAndPtr.first;
			}
		}
		
		// Unfilter & Fill Final Image With Pixels
		// Create Final Image Buffer
		this->m_pBuff = std::make_unique<Coloru8[]>(this->m_nPixels);
		const size_t channelCount  = (colorType == 0 ? 1u : (colorType == 2 ? 3 : (colorType == 3 ? 4 : (colorType == 4 ? 2 : 4u))));
		const size_t bytesPerPixel = bitDepth * channelCount;

		std::cout << "bpp: " << bytesPerPixel << '\n';

		uint16_t uncompressedDataOffset = 0u;
		size_t i = 0u;
		for (uint16_t y = 0; y < this->m_height; y++) {
			const uint8_t scanlineFilterType = uncompressedIdatData[uncompressedDataOffset++];
			for (uint16_t x = 0; x < this->m_width; x++) {
				Coloru8 pixelColor = *(uint32_t*)uncompressedIdatData[uncompressedDataOffset];
				// TODO:: HANDLE NON 8 BIT COLORS

				switch (scanlineFilterType) {
				case 0: // NONE
					break;
				case 1: // Sub (LEFT)
					if (x == 0) break;

					{
						const Coloru8 leftPixel = this->m_pBuff[i - 1u];

						pixelColor += leftPixel;
					}
					
					break;
				case 2: // Up
					break;
				case 3: // Average
					break;
				case 4: // Paeth
					break;
				default:
					WS_THROW("[WS] Your scanline filter type isn't supported");
				}

				this->m_pBuff[i] = pixelColor;

				uncompressedDataOffset += bytesPerPixel;
			}
		}

		delete[] rawCombinedIdatData;  // Dynamic #1
		delete[] uncompressedIdatData; // Dynamic #2
    }

    void Image::Write(const char* filepath) WS_NOEXCEPT
    {
		
    }

}; // WS