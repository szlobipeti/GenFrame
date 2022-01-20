#include "BinaryRead.h"

gen::bin::reader::reader(std::ifstream* inFile)
{
	this->inFile = inFile;
	this->ownsStream = false;
	this->beginOffset = inFile->tellg();
	inFile->seekg(0, std::ios::end);
	this->endOffset = inFile->tellg();

	this->setSize();
	this->seek(0);
	return;
}

gen::bin::reader::reader(std::ifstream* inFile, std::streamoff beginOffset, std::streamoff endOffset)
{
	this->inFile = inFile;
	this->ownsStream = false;

	// std::streamoff cur = inFile->tellg();
	inFile->seekg(0, std::ios::end);
	std::streamoff end = inFile->tellg();

	this->beginOffset = std::clamp(beginOffset, (std::streamoff)0, end);
	this->endOffset = (endOffset > end) ? end : endOffset;

	this->setSize();
	this->seek(0);
	return;
}

gen::bin::reader::reader(std::filesystem::path inFilePath)
{
	this->inFile = new std::ifstream();
	this->ownsStream = true;

	inFile->open(inFilePath, std::ios::in | std::ios::ate | std::ios::binary);
	if (!inFile->is_open())
	{
		return;
	}

	this->endOffset = inFile->tellg();

	this->setSize();
	this->seek(0);
	return;
}

gen::bin::reader::reader(std::filesystem::path inFilePath, std::streamoff beginOffset, std::streamoff endOffset)
{
	this->inFile = new std::ifstream();
	this->ownsStream = true;

	inFile->open(inFilePath, std::ios::in | std::ios::ate | std::ios::binary);
	if (!inFile->is_open())
	{
		return;
	}

	std::streamoff end = inFile->tellg();

	this->beginOffset = std::clamp(beginOffset, (std::streamoff)0, end);
	this->endOffset = (endOffset > end) ? end : endOffset;

	this->setSize();
	this->seek(0);
}

gen::bin::reader::~reader()
{
	if (ownsStream && this->inFile->is_open())
	{
		this->inFile->close();
	}
}

void gen::bin::reader::seek(std::streamoff offset, std::ios_base::seekdir direction)
{
	switch (direction)
	{
	case std::ios_base::beg:
	{
		std::streamoff off = beginOffset + offset;
		off = std::clamp(off, beginOffset, endOffset);
		inFile->seekg(off);
		return;
	}
	break;
	case std::ios_base::end:
	{
		std::streamoff off = endOffset - offset;
		off = std::clamp(off, beginOffset, endOffset);
		inFile->seekg((off < beginOffset) ? beginOffset : off);
		return;
	}
	break;
	case std::ios_base::cur:
	{
		std::streamoff off = inFile->tellg() + offset;
		off = std::clamp(off, beginOffset, endOffset);
		inFile->seekg(off);
		return;
	}
	break;
	default:
		break;
	}
	return;
}
