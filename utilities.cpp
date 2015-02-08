#include "utilities.h"

template<typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

// explicit template declarations (to satify linker)
template std::string to_string<int>(int value);
template std::string to_string<char*>(char* value);


std::string getAppResourcePath()
{
	#ifdef __APPLE__
	char dir_path[1024];
	uint32_t size = sizeof(dir_path);
	if (_NSGetExecutablePath(dir_path, &size) == 0) {
		std::string new_path = to_string(dir_path);
		new_path = new_path.substr(0, new_path.find_last_of('/'));
		new_path = new_path.substr(0, new_path.find_last_of('/'));
		new_path += "/Resources";
        return new_path;
	}
	#endif

	return to_string(".");
}


template<typename T>
std::string getResourcePath(T relative)
{
	std::string base = getAppResourcePath();
	base += to_string(relative);

	return base;
}

// explicit template declarations (to satify linker)
template std::string getResourcePath<const char*>(const char* relative);
template std::string getResourcePath<char*>(char* relative);