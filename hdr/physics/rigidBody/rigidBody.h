
#ifndef RIGID_BODY_H
# define RIGID_BODY_H

# include "hitbox.h"

class RigidBody : public CustomComponent
{
	private:
		uint32_t ownKey = 0;
	public:
		uint32_t GetKey() {return (ownKey);};
		void Init(void *data, size_t size) override;
};

#endif
