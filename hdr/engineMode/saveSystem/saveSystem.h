
#ifndef SAVE_FILE_SYSTEM_H
# define SAVE_FILE_SYSTEM_H

//save file structure:
//[ hash  ][ size  ] [data ]
//[64-bits][32-bits][n-bits]

void SaveStateToFile(const char *file, void *data, size_t size);
void *LoadStateFromFile(const char *file);

void *DataPrepping(void *data, uint32_t size, uint64_t hash);
bool CorruptionCheck(void *data);

#endif
