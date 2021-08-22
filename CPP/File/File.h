#pragma once

#include <fstream>
#include <filesystem>

namespace gen
{
namespace file
{

class File
{
public: enum class eStatus
{
	Uninitialized,
	Valid,
	Invalid
} status = eStatus::Uninitialized;

public: enum class eError
{
	NoError,
	FileNotFound,
	FileNotOpen,
	CouldNotReadFile,
	CouldNotWriteFile
} error = eError::NoError;

	  // Pure Virtual Read function
protected: virtual bool oRead(std::ifstream& inFile, size_t dataBegin, size_t dataSize) = 0;

		 // Pure Virtual Write function
protected: virtual bool oWrite(std::ofstream& outFile) = 0;

		 // Default implementation of the Write virtual function
protected: virtual bool vWrite(std::ofstream& outFile, void* additionalParameters);

		 // Reads the file from the given file path
public: bool Read(std::filesystem::path filePath);

	  // Reads the file from the given inFile ifstream to the end of the ifstream
public: bool Read(std::ifstream& inFile);

	  // Reads the file from the given inFile ifstream for dataSize bytes
public: bool Read(std::ifstream& inFile, size_t dataSize);

	  // Reads the file from the given inFile ifstream, beginning at dataBegin for dataSize bytes
public: bool Read(std::ifstream& inFile, size_t dataBegin, size_t dataSize);

	  // Writes the file to the given file path
public: bool Write(std::filesystem::path filePath);

	  // Writes the file to the given outFile ofstream to the end of the ofstream
public: bool Write(std::ofstream& outFile);

	  // Writes the file to the given file path with additional Parameters
public: bool Write(std::filesystem::path filePath, void* additionalParameters);

	  // Writes the file to the given outFile ofstream to the end of the ofstream with additional Parameters
public: bool Write(std::ofstream& outFile, void* additionalParameters);
};

}
}