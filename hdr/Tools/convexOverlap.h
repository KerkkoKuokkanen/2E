
#ifndef CONVEX_OVERLAP_H
# define CONVEX_OVERLAP_H

# include "box.h"

bool ReactangleScreenOverlap(t_BoundingB &rect);
bool ConvexOverlap(t_BoundingB &rect1, t_BoundingB &rect2);

#endif
