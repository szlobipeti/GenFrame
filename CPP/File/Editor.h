#pragma once

#include "File.h"

namespace gen
{
	namespace file
	{
		namespace edit
		{
			template <typename T, typename = typename std::enable_if<std::is_base_of<gen::file::iFile, T>::value>::type>
			class iEditor
			{
			protected: T& file;
			public:	iEditor() = delete;
			public:	iEditor(T& file) : file{ file } {};

			protected: virtual bool oSwapSource(T& File) = 0;

			public: __forceinline bool swapSource(T& File)
			{
				// VALIDATE
				if (this->file.status != gen::file::iFile::status::valid || File.status != gen::file::iFile::status::valid)
				{
					return false;
				}

				return oSwapSource(File);
			}

			protected: virtual bool oSwapConverted(gen::file::iFile& File) = 0;

			public: __forceinline bool swapConverted(gen::file::iFile& File)
			{
				if (this->file.status != gen::file::iFile::status::valid || File.status != gen::file::iFile::status::valid)
				{
					return false;
				}

				return oSwapConverted(File);
			}
			};
		}
	}
}
