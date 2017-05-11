#include "utills.h"

std::string trim(const std::string& str, char trim_ch)
{
	size_t first = str.find_first_not_of(trim_ch);
	if (std::string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(trim_ch);
	return str.substr(first, (last - first + 1));
}
