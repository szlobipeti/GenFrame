#include "FileConverter.h"

gen::file::FileConverter::FileConverter()
{
	return;
}

gen::file::FileConverter::FileConverter(gen::file::File* File1, gen::file::File* File2)
{
	this->File1 = File1;
	this->File2 = File2;
}

void gem::file::FileConverter::vSetFile1(gen::file::File* File1)
{
	this->File1 = File1;
}

void gem::file::FileConverter::vSetFile2(gen::file::File* File2)
{
	this->File2 = File2;
}
