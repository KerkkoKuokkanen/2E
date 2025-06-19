
#include "rigidBody.h"
#include "rigidBodyManager.h"
#include "hitbox.h"
#include "keyboard.h"
#include "vectorTools.h"
#include "deltaTime.h"

static uint32_t RigidBodyKeyGen()
{
	static uint32_t keys = 0;
	keys += 1;
	if (keys > 4294967292)
		keys = 1;
	return (keys);
}

RigidBody::~RigidBody()
{
	RemoveRigidBody(this);
}

void RigidBody::Init(void *data, size_t size)
{
	ownKey = RigidBodyKeyGen();
	if (size <= 0)
		return ;
}

void RigidBody::Start()
{
	AddRigidBody(this);
	Hitbox *used = (Hitbox*)self->GetComponent("Hitbox");
	if (used != NULL)
		position = used->GetPosition();
}

void RigidBody::ResolveCollisions()
{
	Hitbox *used = (Hitbox*)self->GetComponent("Hitbox");
	if (used == NULL)
		return ;
	for (int i = 0; i < 8; i++)
	{
		UpdateRigidBody(used);
		std::vector<std::tuple<Hitbox*, t_Point, float>> collisions = CheckCollisionRigidBody(used);
		if (collisions.size() == 0)
			return ;
		for (auto &[box, dir, depth] : collisions)
		{
			t_Point rect1 = box->GetPosition();
			t_Point rect2 = position;
			t_Point centerVec = {
				rect2.x - rect1.x,
				rect2.y - rect1.y
			};
			if (dir.x * centerVec.x + dir.y * centerVec.y < 0.0f)
			{
				dir.x = -dir.x;
				dir.y = -dir.y;
			}
			if (depth > 0.001f)
			{
				float alignment = velocity.x * dir.x + velocity.y * dir.y;
				if (alignment < 0.0f)
				{
					float vn = (velocity.x * dir.x + velocity.y * dir.y);
					velocity.x -= vn * dir.x * 0.025f;
					velocity.y -= vn * dir.y * 0.025f;

					float correctionDepth = fmax(depth - 0.01f, 0.0f);
					position.x += dir.x * correctionDepth;
					position.y += dir.y * correctionDepth;
				}
			}
		}
	}
	UpdateRigidBody(used);
	if (fabs(velocity.x) < 0.001f) velocity.x = 0;
	if (fabs(velocity.y) < 0.001f) velocity.y = 0;
}

void RigidBody::Update()
{
	if (KeyHeld(SDL_SCANCODE_RIGHT))
		velocity.x = 3.0f;
	else if (KeyHeld(SDL_SCANCODE_LEFT))
		velocity.x = -3.0f;
	else
		velocity.x = 0.0f;
	if (KeyPressed(SDL_SCANCODE_SPACE))
		velocity.y = 6.0f;
}

void RigidBody::UpdateRigidBody(Hitbox *used)
{
	used->Position(position);
}
