#ifndef UTILITIES_H
	#define UTILITIES_H

#include <string>
#include <sstream>

// to_string replacement for a lack of to_string in some compilers
template <typename T>
std::string to_string(T value);

#endif
