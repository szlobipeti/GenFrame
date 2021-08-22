#pragma once

#include "File.h"

namespace gen
{
	namespace file
	{
		class FileConverter
		{
		protected: gen::file::File* File1 = nullptr;
		protected: gen::file::File* File2 = nullptr;

		public: FileConverter();
		public: FileConverter(gen::file::File* File1, gen::file::File* File2);

		protected: virtual bool oSetFile1() = 0;
		protected: virtual bool oSetFile2() = 0;
		protected: virtual bool oConvertFrom1To2() = 0;
		protected: virtual bool oConvertFrom2To1() = 0;
		};
	}
}