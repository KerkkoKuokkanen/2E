
#ifndef LINES_H
# define LINES_H

# include "box.h"

void InitLines();
void DrawLine(t_Point one, t_Point two);
void DrawLineWithColor(t_Point one, t_Point two, glm::vec4 color);

#endif
