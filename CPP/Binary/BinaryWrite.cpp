#include "BinaryWrite.h"

/*
size_t gen::bin::write::BlockSize = 4000000;

bool gen::bin::write::Write(std::ifstream& inFile, std::ofstream& outFile, size_t DataSize)
{
	if (!inFile.is_open() || !outFile.is_open())
	{
		return false;
	}

	if (DataSize > BlockSize)
	{
		size_t rest = DataSize;
		size_t size = gen::bin::write::BlockSize;
		char* buffer = (char*)malloc(gen::bin::write::BlockSize);

		while (rest > 0)
		{
			size = rest < gen::bin::write::BlockSize ? rest : gen::bin::write::BlockSize;
			inFile.read(buffer, size);
			outFile.write(buffer, size);
			rest -= size;
		}
		free(buffer);
		return true;
	}
	else
	{
		char* buffer = (char*)malloc(DataSize);
		inFile.read(buffer, DataSize);
		outFile.write(buffer, DataSize);
		free(buffer);
		return true;
	}
}
*/

gen::bin::writer::writer(std::ofstream* outFile)
{
	this->outFile = outFile;
	this->ownsStream = false;
	this->beginOffset = outFile->tellp();
}

gen::bin::writer::writer(std::ofstream* outFile, std::streamoff sectionBeginOffset, std::streamoff sectionEndOffset)
{
	this->outFile = outFile;
	this->ownsStream = false;
	this->beginOffset = sectionBeginOffset;
	return;
}

gen::bin::writer::writer(std::filesystem::path outFilePath)
{
	this->outFile = new std::ofstream();
	this->ownsStream = true;

	outFile->open(outFilePath, std::ios::out | std::ios::beg | std::ios::trunc | std::ios::binary);
	if (!outFile->is_open())
	{
		return;
	}
}

gen::bin::writer::writer(gen::bin::writer& bin)
{
	this->outFile = bin.getStream();
	this->ownsStream = false;
	this->beginOffset = bin.tell();
}

gen::bin::writer::~writer()
{
	if (ownsStream && outFile->is_open())
	{
		outFile->close();
	}
}

void gen::bin::writer::setReturn(std::streamoff offset)
{
	returnOffset = (offset < beginOffset) ? beginOffset : offset;
	return;
}

void gen::bin::writer::seek(std::streamoff offset, std::ios_base::seekdir direction)
{
	switch (direction)
	{
	case std::ios_base::beg:
	{
		std::streamoff off = beginOffset + offset;
		off = (off < beginOffset) ? beginOffset : off;
		outFile->seekp(off);
		return;
	}
	break;
	case std::ios_base::end:
	{
		outFile->seekp(offset, std::ios::end);
		return;
	}
	break;
	case std::ios_base::cur:
	{
		std::streamoff off = outFile->tellp() + offset;
		off = (off < beginOffset) ? beginOffset : off;
		outFile->seekp(off);
		return;
	}
	break;
	default:
		break;
	}
	return;
}
