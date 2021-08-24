#include <type_traits>

#define GEN_STATIC_ASSERT_IF_POINTER(T)		static_assert(!std::is_pointer<T>::value, "Template class pointer is not allowed, use value instead")
#define GEN_STATIC_ASSERT_IFN_POINTER(T)	static_assert(std::is_pointer<T>::value, "Template class value is not allowed, use pointer instead")
