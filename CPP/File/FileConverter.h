#pragma once

#include "File.h"

namespace gen
{
	namespace file
	{
		namespace convert
		{
			template <typename T, typename = typename std::enable_if<std::is_base_of<gen::file::File, T>::value>::type>
			class FileConverter
			{
			protected: T& Inner;
			protected: gen::file::File& Outer;

			public: FileConverter() = delete;
			public: FileConverter(T& Inner, gen::file::File& Outer) : Inner{ Inner }, Outer{ Outer } {};

			public: __forceinline void SetInner(T& Inner) { this->Inner = Inner; };
			public: __forceinline void SetOuter(gen::file::File& Outer) { this->Outer = Outer; };
			
			// Converts Inner file to Outer file, where Inner file has to be Valid, but Outer doesn't
			public: __forceinline bool ConvertInnerToOuter()
			{
				if (Inner.status == gen::file::File::eStatus::Invalid)
				{
					return false;
				}

				if (oConvertInnerToOuter())
				{
					Outer.status = gen::file::File::eStatus::Valid;
					return true;
				}
				else
				{
					return false;
				}
			};

			// Converts Inner file to Outer file, where both files have to be Valid
			public: __forceinline bool ConvertOuterToInner()
			{
				if (Inner.status == gen::file::File::eStatus::Invalid || Outer.status == gen::file::File::eStatus::Invalid)
				{
					return false;
				}

				return oConvertOuterToInner();
			};

			protected: virtual bool oConvertInnerToOuter() = 0;
			protected: virtual bool oConvertOuterToInner() = 0;
			};

			// Converts from Inner to Outer
			template<typename T, typename I>
			typename std::enable_if<std::is_base_of<FileConverter<I>, T>::value && !std::is_abstract<T>::value, bool>::type Convert(I& Inner, gen::file::File& Outer)
			{
				T converter(Inner, Outer);
				return converter.ConvertInnerToOuter();
			}

			// Converts from Inner to Outer
			template<typename T, typename I>
			typename std::enable_if<std::is_base_of<FileConverter<I>, T>::value && !std::is_abstract<T>::value, bool>::type Convert(gen::file::File& Outer, I& Inner)
			{
				T converter(Inner, Outer);
				return converter.ConvertOuterToInner();
			}
		}
	}
}