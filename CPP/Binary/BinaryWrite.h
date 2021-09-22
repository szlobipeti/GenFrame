#pragma once

#include <fstream>
#include <stdint.h>
#include <string>

namespace gen
{
	namespace binary
	{
		namespace write
		{
			extern size_t BlockSize;

			template<typename T>
			__forceinline typename std::enable_if<std::is_pointer<T>::value, void>::type Write(std::ofstream& outFile, T Value, size_t Size)
			{
				outFile.write(reinterpret_cast<char*>(Value), Size);
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
	}
}