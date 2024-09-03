#include "libft.h"

/* TODO: memmove */
/* Use compound literals instead please! */
void	quat_copy(t_quat *quat_a, t_quat *quat_b)
{
	quat_a->scalar = quat_b->scalar;
	quat_a->i = quat_b->i;
	quat_a->j = quat_b->j;
	quat_a->k = quat_b->k;
}
