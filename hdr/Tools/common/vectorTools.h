
#ifndef VECTOR_TOOLS_H
# define VECTOR_TOOLS_H

# include "box.h"

float VectorMagnitude(t_Point vec);
t_Point VectorRotate(t_Point vec, float angle);
t_Point VectorNormalize(t_Point vec);
float VectorDotProduct(t_Point vec1, t_Point vec2);
t_Point VectorAdd(t_Point vec1, t_Point vec2);
t_Point VectorSubtract(t_Point vec1, t_Point vec2);
t_Point VectorScalar(t_Point vec, float multi);

#endif
