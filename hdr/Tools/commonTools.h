
#ifndef COMMON_TOOLS_H
# define COMMON_TOOLS_H

#include <time.h>

int	figure_the_delay(clock_t start, clock_t end);
void ClearWindow();
float float_rand();
int rounding(float num);
void SetFrameTime(int used);
bool FAlmostEqual(float a, float b, float precision = 0.00001f);

#endif
