#pragma once

#include "BinaryRead.h"
#include "BinaryWrite.h"

namespace gen::bin
{
	// generic functions for binary operations

	// copies the content of the source stream from the current position until the end of the file, then returns to current position
	bool copyFromFile(std::ifstream& sourceStream, std::ofstream& destinationStream);

	// copies the content of the source stream from the current position until the given length, then returns to the current position
	bool copyFromFile(std::ifstream& sourceStream, std::ofstream& destinationStream, std::streamsize length);

	// compares the two files given by the file paths. returns true if the two files are identical.
	bool compare(std::filesystem::path sourceFilePath, std::filesystem::path compareFilePath);

	// compares the two files given by the input streams. returns true if the two files are identical.
	bool compare(std::ifstream& sourceStream, std::ifstream& compareStream);

	// compares the two files given by the input streams from the beginning of the stream for the given length. returns true if the streams are identical.
	bool compare(std::ifstream& sourceStream, std::ifstream& compareStream, std::streamsize compareLength);

	// compares the two binaries. returns true if the binaries are identical. returns true if the streams are identical.
	bool compare(gen::bin::reader& sourceBin, gen::bin::reader& compareBin);

	// compares the two binaries from the beginning of the binary for the given length. returns true if the streams are identical.
	bool compare(gen::bin::reader& sourceBin, gen::bin::reader& compareBin, std::streamsize compareLength);


	// generic functions for binary io operations in big endian

	template<typename T, std::enable_if_t<std::is_integral<T>::value && !std::is_pointer<T>::value, bool> = true>
	__forceinline T& swapEndian(T& ref)
	{
		T tmp = ref;
		for (size_t i = 0; i < sizeof(T); i++)
		{
			*(char*)(&ref + i) = *(char*)(&tmp + sizeof(T) - i - 1);
		}
		return ref;
	}

	namespace big
	{
		template<typename T, std::enable_if_t<std::is_integral<T>::value && !std::is_pointer<T>::value, bool> = true>
		void read(reader& bin, T& ref)
		{
			for (size_t i = 0; i < sizeof(T); i++)
			{
				char* c = (char*)(&ref) + (sizeof(T) - i - 1);
				bin.read(c, 1);
			}
		}

		template<typename T, std::enable_if_t<std::is_integral<T>::value && !std::is_pointer<T>::value, bool> = true>
		void write(writer& bin, T& ref)
		{
			for (size_t i = 0; i < sizeof(T); i++)
			{
				char* c = (char*)(&ref) + (sizeof(T) - i - 1);
				bin.write(c, 1);
			}
		}
	}
}
