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
			protected: T* Inner = nullptr;
			protected: gen::file::File* Outer = nullptr;

			public: FileConverter() = default;
			public: FileConverter(T* Inner, gen::file::File* Outer)
			{
				this->Inner = Inner;
				this->Outer = Outer;
			};

			public: __forceinline void SetInner(T* Inner) { this->Inner = Inner; };
			public: __forceinline void SetOuter(gen::file::File* Outer) { this->Outer = Outer; };

			public: __forceinline bool ConvertInnerToOuter()
			{
				if (Inner == nullptr || Outer == nullptr)
				{
					return false;
				}

				return oConvertInnerToOuter();
			};
			public: __forceinline bool ConvertOuterToInner()
			{
				if (Inner == nullptr || Outer == nullptr)
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
			typename std::enable_if<std::is_base_of<FileConverter<I>, T>::value && !std::is_abstract<T>::value, bool>::type Convert(I* Inner, gen::file::File* Outer)
			{
				T converter;
				converter.SetInner(Inner);
				converter.SetOuter(Outer);
				return converter.ConvertInnerToOuter();
			}

			// Converts from Inner to Outer
			template<typename T, typename I>
			typename std::enable_if<std::is_base_of<FileConverter<I>, T>::value && !std::is_abstract<T>::value, bool>::type Convert(gen::file::File* Outer, I* Inner)
			{
				T converter;
				converter.SetOuter(Outer);
				converter.SetInner(Inner);
				return converter.ConvertOuterToInner();
			}
		}
	}
}