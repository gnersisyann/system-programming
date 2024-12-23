#pragma once
#include <cstdio>
#include <cstdlib>

inline void error_handler(const char* error)
{
    perror(error);
    exit(EXIT_FAILURE);
}