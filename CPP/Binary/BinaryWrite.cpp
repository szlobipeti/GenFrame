#include "BinaryWrite.h"

size_t gen::binary::write::BlockSize = 4000000;

bool gen::binary::write::Write(std::ifstream& inFile, std::ofstream& outFile, size_t DataSize)
{
	if (!inFile.is_open() || !outFile.is_open())
	{
		return false;
	}

	if (DataSize > BlockSize)
	{
		size_t rest = DataSize;
		size_t size = gen::binary::write::BlockSize;
		char* buffer = (char*)malloc(gen::binary::write::BlockSize);

		while (rest > 0)
		{
			size = rest < gen::binary::write::BlockSize ? rest : gen::binary::write::BlockSize;
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
