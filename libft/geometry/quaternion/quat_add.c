#include "libft.h"

void	quat_add(t_quat *quat_a, t_quat *quat_b)
{
	quat_a->scalar += quat_b->scalar;
	quat_a->i += quat_b->i;
	quat_a->j += quat_b->j;
	quat_a->k += quat_b->k;
}

void	quat_substract(t_quat *quat_a, t_quat *quat_b)
{
	quat_a->scalar -= quat_b->scalar;
	quat_a->i -= quat_b->i;
	quat_a->j -= quat_b->j;
	quat_a->k -= quat_b->k;
}

void	quat_sc_mult(t_quat *quat_a, double sc)
{
	quat_a->scalar *= sc;
	quat_a->i *= sc;
	quat_a->j *= sc;
	quat_a->k *= sc;
}
