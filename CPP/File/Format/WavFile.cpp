#include "WavFile.h"
#include "..\..\Binary\GenBinary.h"
namespace bin
{
	using namespace gen::binary::read;
	using namespace gen::binary::read::little;
	using namespace gen::binary::write;
	using namespace gen::binary::write::little;
}

gen::file::format::WavFile::~WavFile()
{
	if (WaveformatEx != nullptr)
	{
		free(WaveformatEx);
	}

	if (WaveData != nullptr)
	{
		free(WaveData);
	}
	return;
}

bool gen::file::format::WavFile::oRead(std::ifstream& inFile, size_t dataBegin, size_t dataSize)
{
	uint32_t size = 0;
	bin::Read(inFile, size);
	if (size != RIFF)
	{
		return false;
	}

	uint32_t FileSize = 0;
	bin::Read(inFile, FileSize);

	bin::Read(inFile, size);
	if (size != WAVE)
	{
		return false;
	}

	bin::Read(inFile, size);
	if (size != fmt_)
	{
		return false;
	}

	bin::Read(inFile, size);

	tWAVEFORMATEX WaveformatTMP;
	bin::Read(inFile, &WaveformatTMP, 18);
	WaveformatTMP.cbSize = size - 18;

	char* WaveformatExTMP = (char*)malloc(WaveformatTMP.cbSize);
	bin::Read(inFile, WaveformatExTMP, WaveformatTMP.cbSize);

	bin::Read(inFile, size);
	if (size == fact)
	{
		bin::Read(inFile, FactChunk.DataSize);
		
		if (FactChunk.Data != nullptr)
		{
			free(FactChunk.Data);
		}

		FactChunk.Data = (char*)malloc(FactChunk.DataSize);
		bin::Read(inFile, FactChunk.Data, FactChunk.DataSize);

		bin::Read(inFile, size);
	}

	if (size == data)
	{
		bin::Read(inFile, WaveDataSize);

		if (WaveData != nullptr)
		{
			free(WaveData);
		}

		WaveData = (char*)malloc(WaveDataSize);
		bin::Read(inFile, WaveData, WaveDataSize);
	}
	else
	{
		return false;
	}

	Waveformat = WaveformatTMP;

	if (WaveformatEx != nullptr)
	{
		free(WaveformatEx);
	}

	WaveformatEx = WaveformatExTMP;

	return true;
}

bool gen::file::format::WavFile::oWrite(std::ofstream& outFile)
{
	if (WaveformatEx == nullptr || WaveData == nullptr)
	{
		return false;
	}

	uint32_t size = 0;

	size = RIFF;
	bin::Write(outFile, size);

	size = 16 /*fourcc*/ + 12 /*sizes*/ + 18 + Waveformat.cbSize + FactChunk.DataSize + (uint32_t)WaveDataSize;
	bin::Write(outFile, size);

	size = WAVE;
	bin::Write(outFile, size);

	size = fmt_;
	bin::Write(outFile, size);

	size = Waveformat.cbSize + 18;
	bin::Write(outFile, size);

	bin::Write(outFile, &Waveformat, 18);
	bin::Write(outFile, WaveformatEx, Waveformat.cbSize);

	size = fact;
	bin::Write(outFile, size);

	bin::Write(outFile, FactChunk.DataSize);
	bin::Write(outFile, FactChunk.Data, FactChunk.DataSize);

	size = data;
	bin::Write(outFile, size);

	bin::Write(outFile, WaveDataSize);
	bin::Write(outFile, WaveData, WaveDataSize);

	return true;
}


//**********************************************************************************
/*
	outFile.write(reinterpret_cast<char*>(&fourcc), sizeof(uint32_t)); // RIFF

	uint32_t size = header.sectionDataSize + 20; // Data Size
	outFile.write(reinterpret_cast<char*>(&size), sizeof(uint32_t)); // Data Size

	fourcc = 1163280727; // WAVE
	outFile.write(reinterpret_cast<char*>(&fourcc), sizeof(uint32_t)); // WAVE

	fourcc = 544501094; // fmt 
	outFile.write(reinterpret_cast<char*>(&fourcc), sizeof(uint32_t)); // fmt

	size = 18 + waveformat.cbSize; // sizeof format
	outFile.write(reinterpret_cast<char*>(&size), sizeof(uint32_t)); // size of format

	outFile.write(reinterpret_cast<char*>(&waveformat), 18); // format

	outFile.write(reinterpret_cast<char*>(waveformatex), waveformat.cbSize); // additional data

	fourcc = 1952670054;
	outFile.write(reinterpret_cast<char*>(&fourcc), sizeof(uint32_t));

	size = 4;
	outFile.write(reinterpret_cast<char*>(&size), sizeof(uint32_t));

	factChunk.sampleLength = (header.sectionDataSize - 18 - waveformat.cbSize) / waveformat.nBlockAlign * *(uint16_t*)waveformatex;
	outFile.write(reinterpret_cast<char*>(&factChunk.sampleLength), sizeof(uint32_t));

	uint32_t DATA = 1635017060; // data
	outFile.write(reinterpret_cast<char*>(&DATA), sizeof(uint32_t)); // data

	size = header.sectionDataSize - 18 - waveformat.cbSize; // length of data blcok
	outFile.write(reinterpret_cast<char*>(&size), sizeof(uint32_t)); // length of data block
	outFile.write(reinterpret_cast<char*>(data), size); // data
	outFile.close();


	return true;
*/

gen::file::format::WavFile::sFactChunk::~sFactChunk()
{
	if (Data != nullptr)
	{
		free(Data);
	}
	return;
}
