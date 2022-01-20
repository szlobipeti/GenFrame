#pragma once

#include <exception>

namespace gen::exc
{
	class notImplementedException : public std::exception
	{
		const char* what() const throw();
	};
}