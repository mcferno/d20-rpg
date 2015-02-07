#ifndef UTILITIES_H
	#define UTILITIES_H

#include <string>
#include <sstream>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

// to_string replacement for a lack of to_string in some compilers
template <typename T>
std::string to_string(T value);

// get system-dependent application resource directory path
std::string getAppResourcePath();

// Converts a app-level filesystem path into a system-dependent path
template <typename T>
std::string getResourcePath(T input);

#endif