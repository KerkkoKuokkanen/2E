
#ifndef HIT_BOX_MANAGER_H
# define HIT_BOX_MANAGER_H

# include "hitbox.h"

void AddHitBox(Hitbox *hitbox);
void RemoveHitBox(Hitbox *hitbox);
std::vector<Hitbox*> CheckCollision(Hitbox *hitbox);

#endif
