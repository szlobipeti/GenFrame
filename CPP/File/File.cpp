#include "File.h"

using namespace gen::file;

// Reads the file from the given file path
bool iFile::read(std::filesystem::path filePath)
{
	if (!std::filesystem::is_regular_file(filePath))
	{
		error = error::fileNotFound;
		status = status::invalid;
		return false;
	}

	fileName = filePath.filename().string();

	gen::bin::reader bin(filePath);
	if (!bin.isOpen())
	{
		error = error::fileNotOpen;
		status = status::invalid;
		return false;
	}

	if (oRead(bin))
	{
		status = status::valid;
		return true;
	}
	else
	{
		error = error::couldNotReadFile;
		status = status::invalid;
		return false;
	}
}

bool iFile::read(gen::bin::reader& bin, std::string fileName)
{
	this->fileName = fileName;

	if (!bin.isOpen())
	{
		error = error::fileNotOpen;
		status = status::invalid;
		return false;
	}

	if (oRead(bin))
	{
		status = status::valid;
		return true;
	}
	else
	{
		error = error::couldNotReadFile;
		status = status::invalid;
		return false;
	}
}

// Reads the file from the given inFile ifstream to the end of the ifstream
/*
bool File::Read(std::ifstream& inFile)
{
	if (!inFile.is_open())
	{
		error = eError::FileNotOpen;
		status = eStatus::Invalid;
		return false;
	}

	size_t dataBegin = (size_t)inFile.tellg();
	inFile.seekg(0, std::ios::end);
	size_t dataSize = (size_t)inFile.tellg() - dataBegin;

	inFile.seekg(dataBegin, std::ios::beg);
	if (oRead(inFile, dataBegin, dataSize))
	{
		status = eStatus::Valid;
		return true;
	}
	else
	{
		error = eError::CouldNotReadFile;
		status = eStatus::Invalid;
		return false;
	}
}
*/

// Reads the file from the given inFile ifstream for dataSize bytes
/*
bool File::Read(std::ifstream& inFile, size_t dataSize)
{
	if (!inFile.is_open())
	{
		error = eError::FileNotOpen;
		status = eStatus::Invalid;
		return false;
	}

	size_t dataBegin = (size_t)inFile.tellg();
	inFile.seekg(0, std::ios::end);

	if (dataSize > (size_t)inFile.tellg() - dataBegin || dataSize == 0)
	{
		error = eError::CouldNotReadFile;
		status = eStatus::Invalid;
		return false;
	}

	inFile.seekg(dataBegin, std::ios::beg);
	if (oRead(inFile, dataBegin, dataSize))
	{
		status = eStatus::Valid;
		return true;
	}
	else
	{
		error = eError::CouldNotReadFile;
		status = eStatus::Invalid;
		return false;
	}
}
*/

// Reads the file from the given inFile ifstream, beginning at dataBegin for dataSize bytes
/*
bool File::Read(std::ifstream& inFile, size_t dataBegin, size_t dataSize)
{
	if (!inFile.is_open())
	{
		error = eError::FileNotOpen;
		status = eStatus::Invalid;
		return false;
	}

	inFile.seekg(dataBegin, std::ios::beg);
	if (oRead(inFile, dataBegin, dataSize))
	{
		status = eStatus::Valid;
		return true;
	}
	else
	{
		error = eError::CouldNotReadFile;
		status = eStatus::Invalid;
		return false;
	}
}
*/

// Writes the file to the given file path
bool iFile::write(std::filesystem::path outDirPath)
{
	if (status == status::invalid)
	{
		error = error::couldNotWriteFile;
		return false;
	}

	if (!std::filesystem::is_directory(outDirPath) && outDirPath != "")
	{
		error = error::fileNotFound;
		return false;
	}

	gen::bin::writer bin(outDirPath / fileName);
	if (!bin.isOpen())
	{
		error = error::fileNotOpen;
		return false;
	}

	if (oWrite(bin))
	{
		return true;
	}
	else
	{
		error = error::couldNotWriteFile;
		std::filesystem::remove(outDirPath / fileName);
		return false;
	}
}

bool iFile::write(gen::bin::writer& bin)
{
	if (status == status::invalid)
	{
		error = error::couldNotWriteFile;
		return false;
	}

	if (!bin.isOpen())
	{
		error = error::fileNotOpen;
		return false;
	}

	if (oWrite(bin))
	{
		return true;
	}
	else
	{
		error = error::couldNotWriteFile;
		return false;
	}
}

// Writes the file to the given outFile ofstream to the end of the ofstream
/*
bool File::Write(std::ofstream& outFile)
{
	if (status == eStatus::Invalid)
	{
		error = eError::CouldNotWriteFile;
		return false;
	}

	if (!outFile.is_open())
	{
		error = eError::FileNotOpen;
		return false;
	}

	if (oWrite(outFile))
	{
		return true;
	}
	else
	{
		error = eError::CouldNotWriteFile;
		return false;
	}
}
*/

// Writes the file to the given file path with additional Parameters
/*
bool File::Write(std::filesystem::path filePath, void* additionalParameters)
{
	if (status == eStatus::Invalid)
	{
		error = eError::CouldNotWriteFile;
		return false;
	}

	std::filesystem::path dir = filePath;
	dir.remove_filename();
	if (!std::filesystem::is_directory(dir) && dir != "")
	{
		error = eError::FileNotFound;
		return false;
	}

	std::ofstream outFile(filePath, std::ios::out | std::ios::beg | std::ios::trunc | std::ios::binary);
	if (!outFile.is_open())
	{
		error = eError::FileNotOpen;
		return false;
	}

	if (additionalParameters != nullptr && vWrite(outFile, additionalParameters))
	{
		outFile.close();
		return true;
	}
	else
	{
		error = eError::CouldNotWriteFile;
		outFile.close();
		return false;
	}
}
*/

// Writes the file to the given outFile ofstream to the end of the ofstream with additional Parameters
/*
bool File::Write(std::ofstream& outFile, void* additionalParameters)
{
	if (status == eStatus::Invalid)
	{
		error = eError::CouldNotWriteFile;
		return false;
	}

	if (!outFile.is_open())
	{
		error = eError::FileNotOpen;
		return false;
	}

	if (additionalParameters != nullptr && vWrite(outFile, additionalParameters))
	{
		return true;
	}
	else
	{
		error = eError::CouldNotWriteFile;
		return false;
	}
}
*/
