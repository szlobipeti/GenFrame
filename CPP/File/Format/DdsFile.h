#pragma once

#include "..\File.h"
#include "..\..\Dependencies\DDS.h"

namespace gen
{
	namespace file
	{
		namespace format
		{
			class DdsFile : public File
			{
			public: DDS_HEADER Header;
			public: char* PixelData = nullptr;
			public: size_t PixelDataSize = 0;

			public: ~DdsFile();

			private: __forceinline size_t oType() override { return (size_t)eType::dds; };
			private: bool oRead(std::ifstream& inFile, size_t dataBegin, size_t dataSize) override;
			private: bool oWrite(std::ofstream& outFile) override;

			public: static void DecompressBC1(const uint32_t width, const uint32_t height, const char* data, char*& scan0, size_t offset = 0);
			public: static void DecompressBC3(const uint32_t width, const uint32_t height, const char* data, char*& scan0, size_t offset = 0);
			};
		}
	}
}