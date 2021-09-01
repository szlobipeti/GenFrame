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
				T& File;
				FileEditor() = delete;
				FileEditor(T& File) : File{ File }
				{};

			protected: virtual bool oSwapFile(T& File) = 0;

			public: __forceinline bool SwapFile(T& File)
			{
				// VALIDATE
				return oSwapFile(File);
			}
			};
		}
	}
}
