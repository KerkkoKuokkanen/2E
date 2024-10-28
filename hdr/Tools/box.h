
#ifndef BOX_H
# define BOX_H

typedef struct	s_Box
{
	float x;
	float y;
	float w;
	float h;
}				t_Box;

typedef struct	s_Point
{
	float x;
	float y;
}				t_Point;

typedef struct s_BoundingB
{
	t_Point leftTop;
	t_Point rightTop;
	t_Point rightBottom;
	t_Point leftBottom;
}				t_BoundingB;

#endif
