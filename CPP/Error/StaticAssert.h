#include <type_traits>

#define GEN_STATIC_ASSERT_NOT_IMPLEMENTED(Function) static_assert(false, "Pure Virtual Function \""#Function"\" is not implemented")