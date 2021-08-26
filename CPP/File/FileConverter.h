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

			public: __forceinline bool ConvertFromInnerToOuter()
			{
				if (Inner == nullptr || Outer == nullptr)
				{
					return false;
				}

				return oConvertFromInnerToOuter();
			};
			public: __forceinline bool ConvertFromOuterToInner()
			{
				if (Inner == nullptr || Outer == nullptr)
				{
					return false;
				}

				return oConvertFromOuterToInner();
			};

			protected: virtual bool oConvertFromInnerToOuter() = 0;
			protected: virtual bool oConvertFromOuterToInner() = 0;
			};

			template<typename T, typename I>
			typename std::enable_if<std::is_base_of<FileConverter<I>, T>::value && !std::is_abstract<T>::value, bool>::type ConvertFromInnerToOuter(I* Inner, gen::file::File* Outer)
			{
				T converter;
				converter.SetInner(Inner);
				converter.SetOuter(Outer);
				return converter.ConvertFromInnerToOuter();
			}

			template<typename T, typename I>
			typename std::enable_if<std::is_base_of<FileConverter<I>, T>::value && !std::is_abstract<T>::value, bool>::type ConvertFromOuterToInner(gen::file::File* Outer, I* Inner)
			{
				T converter;
				converter.SetOuter(Outer);
				converter.SetInner(Inner);
				return converter.ConvertFromOuterToInner();
			}
		}
	}
}