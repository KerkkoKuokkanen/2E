
#include <fstream>
#include "endianess.h"
#include "commonTools.h"
#include "bitCompression.h"

void SaveStateToFile(const char *file, void *data, size_t size)
{
	std::ofstream outFile(file, std::ios::binary);
	if (outFile)
		outFile.write((const char*)data, size);
}

void *LoadStateFromFile(const char *file)
{
	std::ifstream inFile(file, std::ios::binary);
	if (!inFile)
		return (NULL);
	uint32_t size = 0;
	uint64_t hash = 0;
	if (!inFile.read((char*)&hash, sizeof(uint64_t)))
		return (NULL);
	if (!inFile.read((char*)&size, sizeof(uint32_t)))
		return (NULL);
	hash = FromLittleEndian(hash);
	size = FromLittleEndian(size);
	void *ret = malloc(size + sizeof(uint32_t) + sizeof(uint64_t));
	char *reCast = (char*)ret;
	memcpy(reCast, &hash, sizeof(uint64_t));
	memcpy(reCast + sizeof(uint64_t), &size, sizeof(uint32_t));
	if (!inFile.read(reCast + sizeof(uint32_t) + sizeof(uint64_t), size))
	{
		free(ret);
		return (NULL);
	}
	return (ret);
}

void *DataPrepping(void *data, uint32_t size, uint64_t hash)
{
	void *retData = malloc(size + sizeof(uint64_t) + sizeof(uint32_t));
	char *cCast = (char*)retData;
	memcpy(cCast, &hash, sizeof(uint64_t));
	memcpy(cCast + sizeof(uint64_t), &size, sizeof(uint32_t));
	memcpy(cCast + sizeof(uint64_t) + sizeof(uint32_t), data, size);
	free(data);
	data = retData;
	return (retData);
}

bool CorruptionCheck(void *data)
{
	if (data == NULL)
		return (false);
	char *cCast = (char*)data;
	uint64_t hash = 0;
	uint32_t size = 0;
	void *checkData = cCast + sizeof(uint32_t) + sizeof(uint64_t);
	memcpy(&hash, cCast, sizeof(uint64_t));
	memcpy(&size, cCast + sizeof(uint64_t), sizeof(uint32_t));
	uint64_t checkDataHash = HashData64(checkData, size);
	if (checkDataHash == hash)
		return (true);
	return (false);
}
