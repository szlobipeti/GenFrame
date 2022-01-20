#pragma once

#include <fstream>
#include <stdint.h>
#include <string>
#include <filesystem>

namespace gen::bin
{
	/*
		namespace write
		{
			extern size_t BlockSize;

			// Write std::ofstream
			template<typename T>
			__forceinline typename std::enable_if<std::is_pointer<T>::value, void>::type Write(std::ofstream& outFile, T value, size_t Size = sizeof(T))
			{
				outFile.write(reinterpret_cast<char*>(value), Size);
			}

			template<typename T, typename U>
			__forceinline typename std::enable_if<(std::is_pointer<T>::value&& std::is_unsigned<U>::value), bool>::type WriteFixedBlock(std::ofstream& outFile, T Block, U Size)
			{
				if (Block != nullptr)
				{
					return false;
				}

				Write(outFile, &Size, sizeof(U));
				Write(outFile, Block, Size);

				return true;
			}

			__forceinline bool WriteString32(std::ofstream& outFile, std::string String, uint32_t Size = 0)
			{
				if (Size == 0 || Size > String.length())
				{
					Size = (uint32_t)String.length();
				}

				Write(outFile, &Size, 4);
				Write(outFile, &String[0], Size);

				return true;
			}

			bool Write(std::ifstream& inFile, std::ofstream& outFile, size_t DataSize);

			namespace little
			{
				template<typename U>
				__forceinline typename std::enable_if<(!std::is_pointer<U>::value&& std::is_integral<U>::value), void>::type Write(std::ofstream& outFile, U& Value)
				{
					outFile.write(reinterpret_cast<char*>(&Value), sizeof(U));
				}
			}

			namespace big
			{
				template<typename U>
				__forceinline typename std::enable_if<(!std::is_pointer<U>::value&& std::is_integral<U>::value), void>::type Write(std::ofstream& outFile, U& Value)
				{
					for (size_t i = 0; i < sizeof(U); i++)
					{
						gen::binary::write::Write(outFile, (char*)(&Value) + (sizeof(U) - i - 1), 1);
					}
				}
			}
		}
		*/

	class writer
	{
	public: writer(std::ofstream* outFile);
	public: writer(std::ofstream* outFile, std::streamoff beginOffset, std::streamoff endOffset = 0);
	public: writer(std::filesystem::path outFilePath);
	public: ~writer();

	private: std::ofstream* outFile;
	private: std::streamoff beginOffset = 0;
	private: std::streamoff returnOffset = 0;
	private: bool ownsStream = false;

	public: __forceinline bool isOpen() { return outFile->is_open(); }
	// public: __forceinline bool getSectionSize() { return (size_t)getEnd() - (size_t)beginOffset; }

	// public: void __forceinline setSectionBeginOffset() { beginOffset = outFile->tellp(); return; }
	// public: void __forceinline setSectionBeginOffset(std::streamoff offset) { beginOffset = offset; return; }
	// public: std::streamoff __forceinline getBegin() { return beginOffset; }
	// public: void __forceinline seekBegin() { outFile->seekp(beginOffset); return; }

		  // not sure how to handle this correctly
	// public: std::streamoff getEnd();
	// public: void __forceinline seekEnd() { outFile->seekp(0, std::ios::end); return; }

	public: __forceinline void setReturn() { returnOffset = outFile->tellp(); return; }
	public: void setReturn(std::streamoff Offset);
	public: __forceinline std::streamoff getReturn() { return returnOffset - beginOffset; }
	// public: __forceinline void seekReturn() { outFile->seekp(returnOffset); return; }

	public: __forceinline std::streamoff tell() { return outFile->tellp() - beginOffset; }
	public: __forceinline void seek(std::streamoff offset) { seek(offset, std::ios::beg); return; }
	public: void seek(std::streamoff offset, std::ios_base::seekdir direction);

	// write function definitions
	public: template<typename T, std::enable_if_t<!(std::is_pointer<T>::value), bool> = true>
		__forceinline void write(T& ref) { outFile->write(reinterpret_cast<char*>(&ref), sizeof(T)); return; }

	public: template<typename T, std::enable_if_t<!(std::is_pointer<T>::value), bool> = true>
		__forceinline void write(T& ref, size_t count) { outFile->write(reinterpret_cast<char*>(&ref), count); return; }

	public: template <typename T, std::enable_if_t<std::is_pointer<T>::value, bool> = true>
		__forceinline void write(T& ptr) { outFile->write(reinterpret_cast<char*>(ptr), sizeof(T)); return; }

	public: template <typename T, std::enable_if_t<std::is_pointer<T>::value, bool> = true>
		__forceinline void write(T& ptr, size_t count) { outFile->write(reinterpret_cast<char*>(ptr), count); return; }

	public: __forceinline void write(std::string& string, std::streamsize count) { outFile->write(reinterpret_cast<char*>(&string[0]), count); return; }

	public: template<typename T = uint32_t>
		__forceinline void write(std::string& string) { T c = string.size(); outFile->write(reinterpret_cast<char*>(&c), sizeof(T)); outFile->write(reinterpret_cast<char*>(&string[0]), c); return; }

	public: __forceinline std::ofstream* getStream() { return this->outFile; }
	};
}