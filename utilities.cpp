#include "utilities.h"

template<typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

// explicit template declarations
template std::string to_string<int>(int value);
template std::string to_string<char*>(char* value);
