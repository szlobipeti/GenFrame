#include "NotImplementedException.h"

using namespace gen::exception;

const char* NotImplementedException::what() const throw()
{
	return "Pure virtual function not implemented";
}