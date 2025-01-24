
#ifndef SAVING_INTERFACE_H
# define SAVING_INTERFACE_H

# include "sysSaver.h"

void SaveSnapShot(SnapShot snapShot, std::string file);
void SetAskedData(std::string file);
void *CollectAskedState();
void SaveThread();

#endif
