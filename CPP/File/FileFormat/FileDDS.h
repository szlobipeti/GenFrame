#pragma once

#include "..\File.h"
#include "..\..\Dependencies\DDS.h"

namespace gen
{
	namespace file
	{
		namespace format
		{
			class FileDDS : public File
			{
			public: DDS_HEADER Header;
			public: char* PixelData = nullptr;
			public: size_t PixelDataSize = 0;

			public: ~FileDDS();

			private: bool oRead(std::ifstream& inFile, size_t dataBegin, size_t dataSize) override;
			private: bool oWrite(std::ofstream& outFile) override;
			};
		}
	}
}