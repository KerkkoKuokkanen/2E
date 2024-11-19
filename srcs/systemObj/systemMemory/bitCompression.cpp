
#include <zlib.h>
#include <stdlib.h>

void CompressData(const char *input, size_t inputSize, char **output, size_t *outputSize)
{
	uLongf compSize = compressBound(inputSize);
	*output = (char *)malloc(compSize + sizeof(size_t));
	if (compress((Bytef *)(*output + sizeof(size_t)), &compSize, (const Bytef *)input, inputSize) != Z_OK)
	{
		free(*output);
		*output = NULL;
		*outputSize = 0;
		return;
	}
	*(size_t *)*output = inputSize;
	*outputSize = compSize + sizeof(size_t);
}

void DecompressData(const char *input, size_t inputSize, char **output, size_t *outputSize)
{
	size_t originalSize = *(size_t *)input;
	*output = (char *)malloc(originalSize);
	if (uncompress((Bytef *)*output, (uLongf *)&originalSize, (const Bytef *)(input + sizeof(size_t)), inputSize - sizeof(size_t)) != Z_OK)
	{
		free(*output);
		*output = NULL;
	}
	*outputSize = originalSize;
}
