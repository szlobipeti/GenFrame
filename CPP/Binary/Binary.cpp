#include "Binary.h"

bool gen::bin::copyFromFile(std::ifstream& sourceStream, std::ofstream& destinationStream)
{
	if (!sourceStream.is_open() || !destinationStream.is_open())
	{
		return false;
	}

	std::streamoff current = sourceStream.tellg();
	sourceStream.seekg(0, std::ios::end);
	std::streamoff total = sourceStream.tellg();
	total = total - current;

	char* data = (char*)malloc(total);
	sourceStream.read(data, total);
	sourceStream.seekg(current);

	destinationStream.write(data, total);
	return true;
}

bool gen::bin::copyFromFile(std::ifstream& sourceStream, std::ofstream& destinationStream, std::streamsize length)
{
	if (!sourceStream.is_open() || !destinationStream.is_open())
	{
		return false;
	}

	std::streamoff current = sourceStream.tellg();
	sourceStream.seekg(0, std::ios::end);
	std::streamoff total = sourceStream.tellg();
	total = total - current;
	total = (total < length) ? total : length;

	char* data = (char*)malloc(total);
	sourceStream.read(data, total);
	sourceStream.seekg(current);

	destinationStream.write(data, total);
	return true;
}

bool gen::bin::compare(std::filesystem::path sourceFilePath, std::filesystem::path compareFilePath)
{
	gen::bin::reader sourceBin(sourceFilePath);
	gen::bin::reader compareBin(compareFilePath);

	return gen::bin::compare(sourceBin, compareBin);
}

bool gen::bin::compare(std::ifstream& sourceStream, std::ifstream& compareStream)
{
	gen::bin::reader sourceBin(&sourceStream);
	gen::bin::reader compareBin(&compareStream);

	return gen::bin::compare(sourceBin, compareBin);
}

bool gen::bin::compare(std::ifstream& sourceStream, std::ifstream& compareStream, std::streamsize compareLength)
{
	gen::bin::reader sourceBin(&sourceStream);
	gen::bin::reader compareBin(&compareStream);

	return gen::bin::compare(sourceBin, compareBin, compareLength);
}

bool gen::bin::compare(gen::bin::reader& sourceBin, gen::bin::reader& compareBin)
{
	if (sourceBin.getSize() != compareBin.getSize())
	{
		return false;
	}

	return gen::bin::compare(sourceBin, compareBin, sourceBin.getSize());
}

bool gen::bin::compare(gen::bin::reader& sourceBin, gen::bin::reader& compareBin, std::streamsize compareLength)
{
	if (!sourceBin.isOpen() || !compareBin.isOpen())
	{
		return false;
	}

	std::streamoff sourceReturn = sourceBin.tell();
	std::streamoff compareReturn = compareBin.tell();

	sourceBin.seek(0);
	compareBin.seek(0);

	std::streamsize size = sourceBin.getSize();
	size = (compareLength > size) ? size : compareLength;
	char* source = (char*)malloc((size_t)size);
	char* compare = (char*)malloc((size_t)size);

	sourceBin.read(source, size);
	compareBin.read(compare, size);

	sourceBin.seek(sourceReturn);
	compareBin.seek(compareReturn);

	if (std::memcmp(source, compare, (size_t)size) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
