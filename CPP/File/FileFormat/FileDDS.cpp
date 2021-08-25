#include "FileDDS.h"
#include "..\..\Binary\GenBinary.h"
namespace bin = gen::binary::read::little;

gen::file::format::FileDDS::~FileDDS()
{
	if (PixelData != nullptr)
	{
		free(PixelData);
	}
	return;
}

bool gen::file::format::FileDDS::oRead(std::ifstream& inFile, size_t dataBegin, size_t dataSize)
{
	if (dataSize < 128)
	{
		return false;
	}

	uint32_t Magic = 0;
	// inFile.read(reinterpret_cast<char*>(&Magic), 4);
	bin::Read(inFile, &Magic, 4);

	if (Magic != DDS_MAGIC)
	{
		return false;
	}

	// inFile.read(reinterpret_cast<char*>(&Header), 124);
	bin::Read(inFile, &Header, 124);

	PixelDataSize = dataSize - 128;
	PixelData = (char*)malloc(PixelDataSize);
	// inFile.read(PixelData, PixelDataSize);
	bin::Read(inFile, PixelData, PixelDataSize);

	return true;
}

bool gen::file::format::FileDDS::oWrite(std::ofstream& outFile)
{
	uint32_t Magic = DDS_MAGIC;
	outFile.write(reinterpret_cast<char*>(&Magic), 4);

	outFile.write(reinterpret_cast<char*>(&Header), 124);
	outFile.write(PixelData, PixelDataSize);

	return true;
}
