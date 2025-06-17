
#include "rigidBody.h"

static uint32_t RigidBodyKeyGen()
{
	static uint32_t keys = 0;
	keys += 1;
	if (keys > 4294967292)
		keys = 1;
	return (keys);
}

void RigidBody::Init(void *data, size_t size)
{
	ownKey = RigidBodyKeyGen();
	if (size <= 0)
		return ;
}
