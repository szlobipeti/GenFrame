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
