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

#ifdef __WEISS__OS_WINDOWS

    Image::Image(const char* filepath) WS_NOEXCEPT
    {
        Microsoft::WRL::ComPtr<IWICBitmapSource>      decodedConvertedFrame;
		Microsoft::WRL::ComPtr<IWICBitmapDecoder>     bitmapDecoder;
		Microsoft::WRL::ComPtr<IWICImagingFactory>    factory;
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frameDecoder;

		if (WIN_FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory))))
			WS_THROW("[WS] --> [WIC] Could Not Create IWICImagingFactory");

        { // Convert from char* to wchar_t* & CreateDecoderFromFilename
            const size_t length = mbstowcs(nullptr, filepath, 0);
            wchar_t* filepathW = new wchar_t[length];
            mbstowcs(filepathW, filepath, length);

            if (WIN_FAILED(factory->CreateDecoderFromFilename(filepathW, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &bitmapDecoder)))
                WS_THROW("[WS] --> [WIC] Could Not Read / Open Image");

            delete[] filepathW;
        }

		if (WIN_FAILED(bitmapDecoder->GetFrame(0, &frameDecoder)))
			WS_THROW("[WS] --> [WIC] Could Not Get First Frame Of Image");

		if (WIN_FAILED(frameDecoder->GetSize((UINT*)&this->m_width, (UINT*)&this->m_height)))
			WS_THROW("[WS] --> [WIC] Could Not Get Image Width/Height");

		this->m_nPixels = this->m_width * this->m_height;

	    if (WIN_FAILED(WICConvertBitmapSource(GUID_WICPixelFormat32bppRGBA, frameDecoder.Get(), &decodedConvertedFrame)))
	    	WS_THROW("[WS] --> [WIC] Could Not Create Bitmap Converter");

		this->m_pBuff = std::make_unique<Coloru8[]>(this->m_nPixels * sizeof(WS::Coloru8));

		const WICRect sampleRect{ 0, 0, this->m_width, this->m_height };

		if (WIN_FAILED(decodedConvertedFrame->CopyPixels(&sampleRect, this->m_width * sizeof(WS::Coloru8), this->m_nPixels * sizeof(WS::Coloru8), (BYTE*)this->m_pBuff.get())))
			WS_THROW("[WS] --> [WIC] Could Not Copy Pixels From Bitmap");
    }

    void Image::Write(const char* filepath) WS_NOEXCEPT
    {
		Microsoft::WRL::ComPtr<IWICImagingFactory>    factory;
		Microsoft::WRL::ComPtr<IWICBitmapEncoder>     bitmapEncoder;
		Microsoft::WRL::ComPtr<IWICBitmapFrameEncode> bitmapFrame;
		Microsoft::WRL::ComPtr<IWICStream>            outputStream;

		if (WIN_FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory))))
			WS_THROW("[WS] --> [WIC] Could Not Create IWICImagingFactory");

		if (WIN_FAILED(factory->CreateStream(&outputStream)))
			WS_THROW("[WS] --> [WIC] Failed To Create Output Stream");

		{ // Convert from char* to wchar_t* & InitializeFromFilename
			const size_t length = mbstowcs(nullptr, filepath, 0);
			wchar_t* filepathW = new wchar_t[length];
			mbstowcs(filepathW, filepath, length);

			if (WIN_FAILED(outputStream->InitializeFromFilename(filepathW, GENERIC_WRITE)))
				WS_THROW("[WS] --> [WIC] Failed To Initialize Output Stream From Filename");

			delete[] filepathW;
		}

		char fileExtension[10u]; // assumes the extension has less than 10-1=9 characters
		{
			std::memset(fileExtension, 0u, 10);
			const size_t offset = std::string_view(filepath).find_last_of('.');
			std::memcpy(fileExtension, filepath + offset, strlen(filepath) - offset + 1);
		}

		// This should be fine thanks to sso (small string optimization)
		const std::unordered_map<std::string, REFGUID> extensionToREFUGUIDMap{
			{ ".bmp", GUID_ContainerFormatBmp  }, { ".png",  GUID_ContainerFormatPng  },
			{ ".ico", GUID_ContainerFormatIco  }, { ".jpeg", GUID_ContainerFormatJpeg },
			{ ".jpg", GUID_ContainerFormatJpeg }, { ".tiff", GUID_ContainerFormatTiff },
			{ ".gif", GUID_ContainerFormatGif  }, { ".wmp",  GUID_ContainerFormatWmp  }
		};

		if (extensionToREFUGUIDMap.find(fileExtension) == std::end(extensionToREFUGUIDMap))
			WS_THROW("[WS] --> [WIC] Your Image Extension Is Not Supported");

		if (WIN_FAILED(factory->CreateEncoder(extensionToREFUGUIDMap.at(fileExtension), NULL, &bitmapEncoder)))
			WS_THROW("[WS] --> [WIC] Failed To Create Bitmap Encoder");

		if (WIN_FAILED(bitmapEncoder->Initialize(outputStream.Get(), WICBitmapEncoderNoCache)))
			WS_THROW("[WS] --> [WIC] Failed To Initialize Bitmap ");

		if (WIN_FAILED(bitmapEncoder->CreateNewFrame(&bitmapFrame, NULL)))
			WS_THROW("[WS] --> [WIC] Failed To Create A New Frame");

		if (WIN_FAILED(bitmapFrame->Initialize(NULL)))
			WS_THROW("[WS] --> [WIC] Failed To Initialize A Bitmap's Frame");

		if (WIN_FAILED(bitmapFrame->SetSize(this->m_width, this->m_height)))
			WS_THROW("[WS] --> [WIC] Failed To Set A Bitmap's Frame's Size");

		WICPixelFormatGUID pixelFormat = GUID_WICPixelFormat32bppBGRA;
		if (WIN_FAILED(bitmapFrame->SetPixelFormat(&pixelFormat)))
			WS_THROW("[WS] --> [WIC] Failed To Set Pixel Format On A Bitmap Frame's");

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat32bppBGRA))
			WS_THROW("[WS] --> [WIC] The Requested Pixel Format Is Not Supported");

		const UINT stride     = this->m_width   * sizeof(WS::Coloru8);
		const UINT bufferSize = this->m_nPixels * sizeof(WS::Coloru8);
		if (WIN_FAILED(bitmapFrame->WritePixels(this->m_height, stride, bufferSize, (BYTE*)this->m_pBuff.get())))
			WS_THROW("[WS] --> [WIC] Failed To Write Pixels To A Bitmap's Frame");

		if (WIN_FAILED(bitmapFrame->Commit()))
			WS_THROW("[WS] --> [WIC] Failed To Commit A Bitmap's Frame");

		if (WIN_FAILED(bitmapEncoder->Commit()))
			WS_THROW("[WS] --> [WIC] Failed To Commit Bitmap Encoder");
    }

#else // end of #ifdef __WEISS__OS_WINDOWS

    #pragma message("[WS] Image loading & writing is not supported on your platform! (Yet)")

#endif

}; // WS