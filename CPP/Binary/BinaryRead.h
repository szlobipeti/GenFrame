#include <fstream>
#include <stdint.h>
#include <string>
#include "..\Error\StaticAssert.h"

namespace gen
{
	namespace binary
	{
		namespace read
		{
			namespace little
			{
				// Default Templates
				
				// Read Type from inFile for sizeof Type
				template <typename T>
				typename std::enable_if<true> check()
				{
					bool test(T in)
					{
						return false;
					}

					inline void Read(std::ifstream & inFile, T Type)
					{
						inFile.read(reinterpret_cast<char*>(&Type), sizeof(T));
					}
				}

				inline void Read(std::ifstream& inFile, T Type)
				{
					inFile.read(reinterpret_cast<char*>(&Type), sizeof(T));
				}
				// Read type from inFile for the specified size
				template <class T>
				inline void Read<T, std::enable_if<std::is_pointer<T>::value>>(std::ifstream& inFile, T Type, size_t size)
				{
					inFile.read(reinterpret_cast<char*>(&Type), size);
				}

				// Template specifications

				/*
				template <>
				inline void Read<char*,>(std::ifstream& inFile, char* Value)
				{
					return;
				}

				template <>
				inline void Read<char*>

				template <>
				inline void Read<char*>(std::ifstream& inFile, char* Value, size_t size)
				{
					inFile.read(Value, size);
				}

				// Read string from inFile for sizeof size_t
				template <>
				inline void Read<std::string>(std::ifstream& inFile, std::string Value)
				{
					size_t size = 0;
					Read(inFile, size);
					Value.resize(size);
					Read(inFile, &Value[0], size);
				}
				*/
			}
		}
	}
}