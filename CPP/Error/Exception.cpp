#include "Exception.h"

using namespace gen::exc;

const char* notImplementedException::what() const throw()
{
	return "Pure virtual function not implemented";
}