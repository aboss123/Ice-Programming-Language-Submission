#pragma once
#include "common.h"

void* __reallocvec__(void* buf, size_t buf_size)
{	size_t size, cap;
	if (!buf) size = 0, cap = 1;
	else (size = vec_size(buf), cap = vec_cap(buf), cap *= 2);
	size_t* b = realloc(buf ? raw_p(buf) : 0, buf_size * cap + sizeof(size_t) * 2);
	if (b)
	{
		b[0] = size;
		b[1] = cap;
		return b + 2;
	}
	else {
		// Fatal!
		perror("Internal Error!");
	}
	return NULL;
}


int num_len(unsigned d)
{
	if      (d >= 10000000000) return 11;
	else if (d >= 1000000000) return 10;
	else if (d >= 100000000) return 9;
	else if (d >= 10000000) return 8;
	else if (d >= 1000000) return 7;
	else if (d >= 100000) return 6;
	else if (d >= 10000) return 5;
	else if (d >= 1000) return 4;
	else if (d >= 100) return 3;
	else if (d >= 10) return 2;
	return 1;
}

const char *ntos(unsigned d)
{
	int len = 4096;
	char *c = malloc(len);
	if (c == NULL)
	{
		return NULL;
	}
	sprintf_s(c, len, "%d", d);
	char *ref = c;
	c = __salloc__(c);
	free(ref);
	ref = NULL;
	encode(int, len, c, 1);
	return c;
}