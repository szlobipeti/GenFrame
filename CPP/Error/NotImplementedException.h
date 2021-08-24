#pragma once

#include <exception>

namespace gen
{
namespace exception
{
class NotImplementedException : public std::exception
{
	const char* what() const throw();
};
}
}