
#ifndef COMMON_TOOLS_H
# define COMMON_TOOLS_H

# define PI 3.1415926
# include <time.h>

float float_rand();
int rounding(float num);
bool FAlmostEqual(float a, float b, float precision = 0.00001f);
uint64_t HashData(void *data, size_t length);

#endif
