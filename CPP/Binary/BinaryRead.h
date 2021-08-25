#pragma once

#include <fstream>
#include <stdint.h>
#include <string>

namespace gen
{
	namespace binary
	{
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
			__forceinline bool ReadString(std::ifstream& inFile, std::string String, size_t Size)
			{
				if (Size == 0)
				{
					return false;
				}

				if (Size > sizeof(size_t))
				{
					Size = sizeof(size_t);
				}

				size_t Count = 0;

				Read(inFile, &Count, Size);
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
	}
}