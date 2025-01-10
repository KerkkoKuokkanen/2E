
#include <time.h>
#include <random>
#include "xxhash.h"

uint16_t HashData16(const void *data, size_t length)
{
	uint32_t fullHash = XXH32(data, length, 0);
	return ((uint16_t)(fullHash & 0xFFFF));
}

uint32_t HashData32(const void *data, size_t length)
{
	return (XXH32(data, length, 0));
}

uint64_t HashData64(const void *data, size_t length)
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
