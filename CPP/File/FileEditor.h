#pragma once

#include "File.h"

namespace gen
{
	namespace file
	{
		namespace edit
		{
			template <typename T, typename = typename std::enable_if<std::is_base_of<gen::file::File, T>::value>::type>
			class FileEditor
			{
			protected: T& File;
			public:	FileEditor() = delete;
			public:	FileEditor(T& File) : File{ File } {};

			protected: virtual bool oSwapSource(T& File) = 0;

			public: __forceinline bool SwapSource(T& File)
			{
				// VALIDATE
				if (this->File.status != gen::file::File::eStatus::Valid || File.status != gen::file::File::eStatus::Valid)
				{
					return false;
				}

				return oSwapSource(File);
			}

			protected: virtual bool oSwapConverted(gen::file::File& File) = 0;

			public: __forceinline bool SwapConverted(gen::file::File& File)
			{
				if (this->File.status != gen::file::File::eStatus::Valid || File.status != gen::file::File::eStatus::Valid)
				{
					return false;
				}

				return oSwapConverted(File);
			}
			};
		}
	}
}
