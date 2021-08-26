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
			};
		}
	}
}