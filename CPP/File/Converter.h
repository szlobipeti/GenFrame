#pragma once

#include "File.h"

namespace gen::file::conv
{
	template <typename T, typename = typename std::enable_if<std::is_base_of<gen::file::iFile, T>::value>::type>
	class iConverter
	{
	protected: T& inner;
	protected: gen::file::iFile& outer;

	public: iConverter() = delete;
	public: iConverter(T& inner, gen::file::iFile& outer) : inner { inner }, outer { outer } {};

	public: __forceinline void setInner(T& inner) { this->inner = inner; };
	public: __forceinline void setOuter(gen::file::iFile& outer) { this->outer = outer; };

		  // Converts Inner file to Outer file, where Inner file has to be Valid, but Outer doesn't
	public: __forceinline bool convertInnerToOuter()
	{
		if (inner.status == gen::file::status::invalid)
		{
			return false;
		}

		if (oConvertInnerToOuter())
		{
			outer.status = gen::file::status::valid;
			return true;
		}
		else
		{
			return false;
		}
	};

		  // Converts Inner file to Outer file, where both files have to be Valid
	public: __forceinline bool convertOuterToInner()
	{
		if (inner.status == gen::file::status::invalid || outer.status == gen::file::status::invalid)
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
	typename std::enable_if<std::is_base_of<iConverter<I>, T>::value && !std::is_abstract<T>::value, bool>::type convert(I& inner, gen::file::iFile& outer)
	{
		T converter(inner, outer);
		return converter.convertInnerToOuter();
	}

	// Converts from Inner to Outer
	template<typename T, typename I>
	typename std::enable_if<std::is_base_of<iConverter<I>, T>::value && !std::is_abstract<T>::value, bool>::type convert(gen::file::iFile& outer, I& inner)
	{
		T converter(inner, outer);
		return converter.convertOuterToInner();
	}
}