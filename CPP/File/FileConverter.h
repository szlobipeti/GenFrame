#pragma once

#include "File.h"

namespace gen
{
	namespace file
	{
		class FileConverter
		{
		private: gen::file::File* File1 = nullptr;
		private: gen::file::File* File2 = nullptr;

		public: FileConverter();
		public: FileConverter(gen::file::File* File1, gen::file::File* File2);

		protected: virtual void vSetFile1(gen::file::File* File1);
		protected: virtual void vSetFile2(gen::file::File* File2);
		protected: virtual bool oConvertFrom1To2() = 0;
		protected: virtual bool oConvertFrom2To1() = 0;
		};
	}
}