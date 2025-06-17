
#include "rigidBodyManager.h"
#include "rigidBody.h"
#include "screen.h"

std::vector<std::tuple<uint32_t, RigidBody*>> rigidBodies = {};

void AddRigidBody(RigidBody *body)
{
	rigidBodies.push_back({body->GetKey(), body});
}

void RemoveRigidBody(RigidBody *body)
{
	uint32_t key = body->GetKey();
	for (auto &[k, b] : rigidBodies)
	{
		if (k == key)
		{
			k = 0;
			return ;
		}
	}
}

void UpdateRigidBody()
{
	rigidBodies.erase(std::remove_if(rigidBodies.begin(), rigidBodies.end(),
					[](std::tuple<uint32_t, RigidBody*> data) {
						return std::get<0>(data) == 0;
					}), rigidBodies.end());

	/*
	[Clear Forces]
		↓
	[Integrate Forces]
		↓
	[Predict Position]
		↓
	[Detect Collisions]
		↓
	[Solve Constraints]
	*/
}
