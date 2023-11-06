#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#include <iostream>

//  If defined, output from 'DEBUT_PRINT' (see below) will be RED
#ifdef RED_DEBUG_OUTPUT
#define ANSI_COLOR_INSERT "\033[31m" <<
#define ANSI_RESET_INSERT "\033[0m" <<
#else
#define ANSI_COLOR_INSERT
#define ANSI_RESET_INSERT
#endif

//  If defined, output from 'DEBUT_PRINT' (see below) will be BLUE
#ifdef BLUE_DEBUG_OUTPUT
#define ANSI_COLOR_INSERT "\033[34m" <<
#define ANSI_RESET_INSERT "\033[0m" <<
#else
#define ANSI_COLOR_INSERT
#define ANSI_RESET_INSERT
#endif

//  Output to print to console/std::out
#ifdef DEBUG
#define DEBUG_PRINT(...) print(__VA_ARGS__);
#else
#define DEBUG_PRINT(...)
#endif


/** \brief Base case for recursively printing function 'print(...)' */
static void print() {
    std::cout << std::endl;
}

/** \brief Variadic template function to print multiple passed arguments. */
template<typename T, typename... Args>
static void print(T first, Args... rest) {
    std::cout << ANSI_COLOR_INSERT first << ANSI_RESET_INSERT "";
    print(rest...);  // Recursive call with one less argument
}


#endif  //  DEBUG_MACROS_H