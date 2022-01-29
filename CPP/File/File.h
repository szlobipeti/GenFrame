#pragma once

#include <fstream>
#include <filesystem>
#include "..\Binary\GenBin.h"

namespace gen
{
namespace file
{
enum class type : size_t
{
	NONE,
	bin,
	dds,
	wav,
	obj,
	fbx,
	typeSize
};

enum class status
{
	valid,
	invalid
};

enum class error
{
	noError,
	fileNotFound,
	fileNotOpen,
	couldNotReadFile,
	couldNotWriteFile,
};

class iFile
{
public: status status = status::valid;

public: error error = error::noError;

public: std::string fileName = "unk.bin";

	  // Pure Virtaul Get Type function
protected: __forceinline virtual size_t oType() = 0;

		 // Gets the Type of the file
public: __forceinline size_t type() { return oType(); };

	  // Pure Virtual Read function
protected: virtual bool oRead(gen::bin::reader& bin) = 0;

		 // Pure Virtual Write function
protected: virtual bool oWrite(gen::bin::writer& bin) = 0;


		 // Reads the file from the given file path
public: bool read(std::filesystem::path inFilePath);

	  // Reads the file from the given reader
public: bool read(gen::bin::reader& bin, std::string fileName);

	  /*
	  // Reads the file from the given inFile ifstream to the end of the ifstream
public: bool Read(std::ifstream& inFile);

	  // Reads the file from the given inFile ifstream for dataSize bytes
public: bool Read(std::ifstream& inFile, size_t dataSize);

	  // Reads the file from the given inFile ifstream, beginning at dataBegin for dataSize bytes
public: bool Read(std::ifstream& inFile, size_t dataBegin, size_t dataSize);
	  */

	  // Writes the file to the given file path
public: bool write(std::filesystem::path outDirPath);

	  // Writes the file to the given writer
public: bool write(gen::bin::writer& bin);

	  /*
	  // Writes the file to the given outFile ofstream to the end of the ofstream
public: bool Write(std::ofstream& outFile);

	  // Writes the file to the given file path with additional Parameters
public: bool Write(std::filesystem::path filePath, void* additionalParameters);

	  // Writes the file to the given outFile ofstream to the end of the ofstream with additional Parameters
public: bool Write(std::ofstream& outFile, void* additionalParameters);
	  */
};

}
}