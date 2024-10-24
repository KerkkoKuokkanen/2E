
#include <time.h>
#include <OpenGL/gl3.h>
#include <random>

static int usedFrame = 17;

bool FAlmostEqual(float a, float b, float precision = 0.000001f)
{
	return (std::abs(a - b) < precision);
}

int	figure_the_delay(clock_t start, clock_t end)
{
	double	time;
	int		ret;
	time = 0.0;
	time += (double)(end - start) / CLOCKS_PER_SEC;
	ret = usedFrame - (int)(time * 1000.0f);
	if (ret < 0)
		return (0);
	return (ret);
}

void ClearWindow()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
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

void SetFrameTime(int used)
{
	usedFrame = used;
}
