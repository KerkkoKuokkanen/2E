
#include "SDL_mixer.h"
#include <chrono>
#include <mutex>

auto startTime = std::chrono::high_resolution_clock::now();

struct Channel
{
	bool occupied;
	double duration;
	uint64_t key;
};

static double DeltaTime()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltaTime = currentTime - startTime;
	startTime = currentTime;
	return (deltaTime.count());
}

static double GetDuration(Mix_Chunk *sound)
{
	uint32_t totalSamples = sound->alen / (2 * 2);
	double duration = (double)totalSamples / 44100.0;
	return (duration + 0.25);
}
