#pragma once

#include <fstream>
#include <stdint.h>
#include <string>
#include <filesystem>

namespace gen::bin
{
	/*
	namespace read
		{
			// Read to Value from inFile for the specified size
			template<typename T>
			__forceinline typename std::enable_if<std::is_pointer<T>::value, void>::type Read(std::ifstream& inFile, T Value, size_t Size)
			{
				inFile.read(reinterpret_cast<char*>(Value), Size);
			}

			// Read an unsigned integer of specified Size for block size, then read the following block to Block. Block has to be a nullptr to be able to read. Size is capped to 8
			template<typename T>
			__forceinline typename std::enable_if<std::is_pointer<T>::value, bool>::type ReadFixedBlock(std::ifstream& inFile, T Block, size_t Size)
			{
				if (Size == 0)
				{
					return false;
				}

				if (Block != nullptr)
				{
					return false;
				}

				if (Size > 8)
				{
					Size = 8;
				}

				uint64_t Count = 0;

				Read(inFile, &Count, Size);
				Block = (T)malloc(Count);
				Read(inFile, Block, Count);

				return true;
			}

			// Read an unsigned integer of specified Size for String legnth, then read the String. Size is capped to size of size_t
			__forceinline bool ReadString32(std::ifstream& inFile, std::string& String)
			{
				uint32_t Count = 0;

				Read(inFile, &Count, 4);
				String.resize(Count);
				Read(inFile, &String[0], Count);

				return true;
			}

			namespace little
			{
				template<typename U>
				__forceinline typename std::enable_if<(!std::is_pointer<U>::value && std::is_integral<U>::value), void>::type Read(std::ifstream& inFile, U& Value)
				{
					inFile.read(reinterpret_cast<char*>(&Value), sizeof(U));
				}		
			}

			namespace big
			{
				template<typename U>
				__forceinline typename std::enable_if<(!std::is_pointer<U>::value&& std::is_integral<U>::value), void>::type Read(std::ifstream& inFile, U& Value)
				{
					for (size_t i = 0; i < sizeof(U); i++)
					{
						gen::binary::read::Read(inFile, (char*)(&Value) + (sizeof(U) - i - 1), 1);
					}
				}
			}
		}
	*/

	class reader
	{
	public: reader(std::ifstream* inFile);
	public: reader(std::ifstream* inFile, std::streamoff beginOffset, std::streamoff endOffset);
	public: reader(std::filesystem::path inFilePath);
	public: reader(std::filesystem::path inFilePath, std::streamoff beginOffset, std::streamoff endOffset);
	public: reader(gen::bin::reader& bin);
	public: ~reader();

	private: std::ifstream* inFile;
	private: std::streamoff beginOffset = 0;
	private: std::streamoff endOffset = 0;
	private: std::streamsize size = 0;
	private: bool ownsStream = false;

	public: __forceinline bool isOpen() { return inFile->is_open(); }
	private: __forceinline void setSize() { size = endOffset - beginOffset; return; }
	public: __forceinline std::streamsize getSize() { return size; }

	// beginOffset methods

	// public: void __forceinline setBegin() { beginOffset = inFile->tellg(); return; }
	// public: void __forceinline setBegin(std::streamoff offset) { beginOffset = offset; return; }
	// public: std::streamoff __forceinline getBegin() { return beginOffset; }
	// public: void __forceinline seekBegin() { inFile->seekg(beginOffset); return; }

	// endOffset methods
	
	// public: void __forceinline setEnd() { endOffset = inFile->tellg(); return; }
	// public: void __forceinline setEnd(std::streamoff offset) { endOffset = offset; return; }
	// public: std::streamoff __forceinline getEnd() { return endOffset; }
	// public: void __forceinline seekEnd() { inFile->seekg(endOffset); return; }

	// returnOffset methods


	public: __forceinline std::streamoff tell() { return inFile->tellg() - beginOffset; }
	public: __forceinline void seek(std::streamoff offset) { seek(offset, std::ios::beg); return; }
	public: void seek(std::streamoff offset, std::ios_base::seekdir direction);

	// read function definitions
	public: template<typename T, std::enable_if_t<!(std::is_pointer<T>::value), bool> = true>
		__forceinline void read(T& ref) { inFile->read(reinterpret_cast<char*>(&ref), sizeof(T)); return; }

	public: template<typename T, std::enable_if_t<!(std::is_pointer<T>::value), bool> = true>
		__forceinline void read(T& ref, std::streamsize count) { inFile->read(reinterpret_cast<char*>(&ref), count); return; }

	public: template <typename T, std::enable_if_t<std::is_pointer<T>::value, bool> = true>
		__forceinline void read(T& ptr) { inFile->read(reinterpret_cast<char*>(ptr), sizeof(T)); return; }

	public: template <typename T, std::enable_if_t<std::is_pointer<T>::value, bool> = true>
		__forceinline void read(T& ptr, std::streamsize count) { inFile->read(reinterpret_cast<char*>(ptr), count); return; }

	public: __forceinline void read(std::string& string, std::streamsize count) { inFile->read(reinterpret_cast<char*>(&string[0]), count); return; }

	public: template<typename T = uint32_t>
		__forceinline void read(std::string& string)
	{
		T c = 0;
		inFile->read(reinterpret_cast<char*>(&c), sizeof(T));
		string.resize(c);
		inFile->read(reinterpret_cast<char*>(&string[0]), c);
		return;
	}

	public: __forceinline std::ifstream* getStream() { return this->inFile; };
	};
}