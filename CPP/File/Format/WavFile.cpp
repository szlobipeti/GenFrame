#include "WavFile.h"
#include "..\..\Binary\GenBin.h"

gen::file::format::wavFile::~wavFile()
{
	if (waveformatEx != nullptr)
	{
		free(waveformatEx);
	}

	if (waveData != nullptr)
	{
		free(waveData);
	}
	return;
}

bool gen::file::format::wavFile::oRead(gen::bin::reader& bin)
{
	uint32_t size = 0;
	bin.read(size);
	if (size != RIFF)
	{
		return false;
	}

	uint32_t fileSize = 0;
	bin.read(fileSize);

	bin.read(size);
	if (size != WAVE)
	{
		return false;
	}

	bin.read(size);
	if (size != fmt_)
	{
		return false;
	}

	bin.read(size);

	tWAVEFORMATEX WaveformatTMP;
	bin.read(WaveformatTMP, 18);
	WaveformatTMP.cbSize = size - 18;

	char* WaveformatExTMP = (char*)malloc(WaveformatTMP.cbSize);
	bin.read(WaveformatExTMP, WaveformatTMP.cbSize);

	bin.read(size);
	if (size == fact)
	{
		bin.read(factChunk.dataSize);
		
		if (factChunk.data != nullptr)
		{
			free(factChunk.data);
		}

		factChunk.data = (char*)malloc(factChunk.dataSize);
		bin.read(factChunk.data, factChunk.dataSize);

		bin.read(size);
	}

	if (size == data)
	{
		bin.read(waveDataSize);

		if (waveData != nullptr)
		{
			free(waveData);
		}

		waveData = (char*)malloc(waveDataSize);
		bin.read(waveData, waveDataSize);
	}
	else
	{
		return false;
	}

	waveformat = WaveformatTMP;

	if (waveformatEx != nullptr)
	{
		free(waveformatEx);
	}

	waveformatEx = WaveformatExTMP;

	return true;
}

bool gen::file::format::wavFile::oWrite(gen::bin::writer& bin)
{
	if (waveformatEx == nullptr || waveData == nullptr)
	{
		return false;
	}

	uint32_t size = 0;

	size = RIFF;
	bin.write(size);

	size = 16 /*fourcc*/ + 12 /*sizes*/ + 18 + waveformat.cbSize + factChunk.dataSize + (uint32_t)waveDataSize;
	bin.write(size);

	size = WAVE;
	bin.write(size);

	size = fmt_;
	bin.write(size);

	size = waveformat.cbSize + 18;
	bin.write(size);

	bin.write(waveformat, 18);
	bin.write(waveformatEx, waveformat.cbSize);

	size = fact;
	bin.write(size);

	bin.write(factChunk.dataSize);
	bin.write(factChunk.data, factChunk.dataSize);

	size = data;
	bin.write(size);

	bin.write(waveDataSize);
	bin.write(waveData, waveDataSize);

	return true;
}

	// THIS FUNCTION MODS THE FILE FORMAT, ONLY USED WITH .NET SOUND API
bool gen::file::format::wavFile::toByteStream(char*& stream)
{
	if (waveformatEx == nullptr || waveData == nullptr)
	{
		return false;
	}

	if (stream != nullptr)
	{
		free(stream);
	}

	stream = (char*)malloc(54 + waveformat.cbSize + factChunk.dataSize + waveDataSize);

	uint32_t size = 0;
	size_t offset = 0;

	size = RIFF;
	*(uint32_t*)(stream + offset) = size;
	offset += 4;

	size = 16 /*fourcc*/ + 12 /*sizes*/ + 18 + waveformat.cbSize + factChunk.dataSize + (uint32_t)waveDataSize;
	*(uint32_t*)(stream + offset) = size;
	offset += 4;

	size = WAVE;
	*(uint32_t*)(stream + offset) = size;
	offset += 4;

	size = fmt_;
	*(uint32_t*)(stream + offset) = size;
	offset += 4;

	// THIS PART MODS THE FORMAT, NOT SAFE FOR EXTERNAL USE
	if (waveformat.wFormatTag == 0xFFFE) // 0xFFFE waveformat extensible
	{
		uint16_t SubFormat = *(uint16_t*)(waveformatEx + 6);
		if (SubFormat == 1 || SubFormat == 2)
		{
			// if the extensible format can be simplified into regular PCM / uncomressed format

			size = 18;
			*(uint32_t*)(stream + offset) = size;
			offset += 4;

			*(tWAVEFORMATEX*)(stream + offset) = waveformat; // 18
			offset += 18;

			// patch format at offset 0 to SubFormat
			*(uint16_t*)(stream + offset - 18) = SubFormat;

			// patch cb size at offset 16 to 0
			*(uint16_t*)(stream + offset - 2) = 0;
		}
	}
	// ****************************************************
	else
	{
		size = waveformat.cbSize + 18;
		*(uint32_t*)(stream + offset) = size;
		offset += 4;

		*(tWAVEFORMATEX*)(stream + offset) = waveformat; // 18
		offset += 18;
		// bin::Write(outFile, &Waveformat, 18);
		std::memcpy(stream + offset, waveformatEx, waveformat.cbSize);
		offset += waveformat.cbSize;
		// bin::Write(outFile, WaveformatEx, Waveformat.cbSize);
	}

	size = fact;
	*(uint32_t*)(stream + offset) = size;
	offset += 4;

	*(uint32_t*)(stream + offset) = factChunk.dataSize;
	offset += 4;
	// bin::Write(outFile, FactChunk.DataSize);

	std::memcpy(stream + offset, factChunk.data, factChunk.dataSize);
	offset += factChunk.dataSize;
	// in::Write(outFile, FactChunk.Data, FactChunk.DataSize);

	size = data;
	*(uint32_t*)(stream + offset) = size;
	offset += 4;
	// bin::Write(outFile, size);

	*(uint32_t*)(stream + offset) = waveDataSize;
	offset += 4;
	// bin::Write(outFile, WaveDataSize);

	std::memcpy(stream + offset, waveData, waveDataSize);
	// bin::Write(outFile, WaveData, WaveDataSize);

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

gen::file::format::wavFile::factChunk::~factChunk()
{
	if (data != nullptr)
	{
		free(data);
	}
	return;
}
