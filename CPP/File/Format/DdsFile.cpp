#include "DdsFile.h"
#include "..\..\Binary\GenBinary.h"
namespace bin
{
	using namespace gen::binary::read;
	using namespace gen::binary::write;
	using namespace gen::binary::read::little;
	using namespace gen::binary::write::little;
}

gen::file::format::DdsFile::DdsFile()
{
	return;
}

gen::file::format::DdsFile::~DdsFile()
{
	if (PixelData != nullptr)
	{
		free(PixelData);
	}
	return;
}

bool gen::file::format::DdsFile::oRead(std::ifstream& inFile, size_t dataBegin, size_t dataSize)
{
	if (dataSize < 128)
	{
		return false;
	}

	uint32_t Magic = 0;
	bin::Read(inFile, Magic);

	if (Magic != DDS_MAGIC)
	{
		return false;
	}

	bin::Read(inFile, &Header, 124);

	PixelDataSize = dataSize - 128;
	PixelData = (char*)malloc(PixelDataSize);
	bin::Read(inFile, PixelData, PixelDataSize);

	return true;
}

bool gen::file::format::DdsFile::oWrite(std::ofstream& outFile)
{
	uint32_t Magic = DDS_MAGIC;
	bin::Write(outFile, Magic);

	bin::Write(outFile, &Header, 124);
	bin::Write(outFile, PixelData, PixelDataSize);

	return true;
}
