#include "../src/utils.h"
#include <unistd.h>

#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

/**
 * Reference: https://www.youtube.com/watch?v=IZiUT-ipnj0&list=WL
 * [1] Uses errno from <errno.h> to trace errors in code. Libraries uses errno to store error information. 
 * [2] __FILE__ is a preprocessor macro for the name of the file
 * [3] __LINE__ is also a macro, but the for the line number at which the macro is executed.
 */
#define returnErrorTrace(...) ({\
    fprintf(stderr, "%s[ERROR ]%s %s (LINE: ~%d): %s\n", F_RED, F_NORMAL, __FILE__, __LINE__, strerror(errno));\
    exit(-1);\
})


#endif 