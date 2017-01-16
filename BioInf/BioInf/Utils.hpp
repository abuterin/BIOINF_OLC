#pragma once
#include <string>
#include <sys/stat.h>
#include "CommonHeaders.hpp"

/*************************************************************************************** /
* Title: RNA Assembler source code
* Author : rvaser (robert.vaser@gmail.com)
* Date : Apr 13, 2015
* Availability : https://github.com/mariokostelac/ra
*************************************************************************************** */

#define ASSERT(expr, location, msg, ...)\
    do {\
        if (!(expr)) {\
            fprintf(stderr, "[ERROR][" location "]: " msg "\n", ##__VA_ARGS__);\
            exit(-1);\
        }\
    } while (0)

typedef unsigned int uint;

std::string reverseComplement(const std::string& original);

void debug(const char* fmt, ...);
