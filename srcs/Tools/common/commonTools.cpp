
#include <time.h>
#include <random>
#include "xxhash.h"

uint64_t HashData(void *data, size_t length)
{
	return (XXH3_64bits(data, length));
}

bool FAlmostEqual(float a, float b, float precision = 0.00001f)
{
	return (std::abs(a - b) < precision);
}

float	float_rand()
{
	float	scale = ((double)rand() / (double)(RAND_MAX));
	return (scale);
}

int rounding(float num)
{
	int fullVersion = (int)num;
	float rest = num - (float)fullVersion;
	int add = (rest >= 0.5f) ? 1 : 0;
	return (fullVersion + add);
}
