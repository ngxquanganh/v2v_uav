#ifdef __cplusplus
extern "C"
{
#endif
#pragma once

#ifndef UTIL_H
#define UTIL_H

#define DEBUG
#include <stdio.h>

#ifdef DEBUG

#define PRINT_DEBUG(...)  \
    printf("\033[0;36m"); \
    printf(__VA_ARGS__);  \
    printf("\033[0m");

    // #define debug_printf(...) (printf("\033[1;33m"); printf( __VA_ARGS__); printf("\033[0m");)
#else
#define PRINT_DEBUG(...)
#endif

#endif
#ifdef __cplusplus
}
#endif
