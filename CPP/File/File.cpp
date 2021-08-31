#include "File.h"

using namespace gen::file;

// Reads the file from the given file path
bool File::Read(std::filesystem::path filePath)
{
	if (status != eStatus::Uninitialized)
	{
		error = eError::FileAlreadyInitialized;
		return false;
	}

	if (!std::filesystem::is_regular_file(filePath))
	{
		error = eError::FileNotFound;
		status = eStatus::Invalid;
		return false;
	}

	std::ifstream inFile(filePath, std::ios::in | std::ios::ate | std::ios::binary);
	if (!inFile.is_open())
	{
		error = eError::FileNotOpen;
		status = eStatus::Invalid;
		return false;
	}

	size_t dataSize = (size_t)inFile.tellg();

	inFile.seekg(0, std::ios::beg);
	if (oRead(inFile, 0, dataSize))
	{
		status = eStatus::Valid;
		inFile.close();
		return true;
	}
	else
	{
		error = eError::CouldNotReadFile;
		status = eStatus::Invalid;
		inFile.close();
		return false;
	}
}

// Reads the file from the given inFile ifstream to the end of the ifstream
bool File::Read(std::ifstream& inFile)
{
	if (status != eStatus::Uninitialized)
	{
		error = eError::FileAlreadyInitialized;
		return false;
	}

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

// Reads the file from the given inFile ifstream for dataSize bytes
bool File::Read(std::ifstream& inFile, size_t dataSize)
{
	if (status != eStatus::Uninitialized)
	{
		error = eError::FileAlreadyInitialized;
		return false;
	}

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

// Reads the file from the given inFile ifstream, beginning at dataBegin for dataSize bytes
bool File::Read(std::ifstream& inFile, size_t dataBegin, size_t dataSize)
{
	if (status != eStatus::Uninitialized)
	{
		error = eError::FileAlreadyInitialized;
		return false;
	}

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

// Writes the file to the given file path
bool File::Write(std::filesystem::path filePath)
{
	if (status == eStatus::Invalid || status == eStatus::Uninitialized)
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

	if (oWrite(outFile))
	{
		outFile.close();
		return true;
	}
	else
	{
		error = eError::CouldNotWriteFile;
		std::filesystem::remove(filePath);
		outFile.close();
		return false;
	}
}

// Writes the file to the given outFile ofstream to the end of the ofstream
bool File::Write(std::ofstream& outFile)
{
	if (status == eStatus::Invalid || status == eStatus::Uninitialized)
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

// Writes the file to the given file path with additional Parameters
bool File::Write(std::filesystem::path filePath, void* additionalParameters)
{
	if (status == eStatus::Invalid || status == eStatus::Uninitialized)
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

// Writes the file to the given outFile ofstream to the end of the ofstream with additional Parameters
bool File::Write(std::ofstream& outFile, void* additionalParameters)
{
	if (status == eStatus::Invalid || status == eStatus::Uninitialized)
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

// Default implementation of the Write virtual function
bool File::vWrite(std::ofstream& outFile, void* additionalParameters)
{
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
