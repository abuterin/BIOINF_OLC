#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "Utils.hpp"

void debug(const char* fmt, ...) {
#ifdef DEBUG
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
#endif
}

std::string reverseComplement(const std::string& original) {

	std::string res;
	res.reserve(original.size());

	for (int i = original.size() - 1; i >= 0; --i) {

		char c = original[i];

		switch (c) {
		case 'A':
			c = 'T';
			break;
		case 'T':
			c = 'A';
			break;
		case 'C':
			c = 'G';
			break;
		case 'G':
			c = 'C';
			break;
		default:
			break;
		}

		res.push_back(c);
	}

	return res;
}