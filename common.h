#pragma once
#pragma warning(disable: 6011)
#pragma warning(disable: 6386)

#include <assert.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "math.h"
#include <stdint.h>
#include <libmlh/LibMLH.h>

/**#define MAX(a,b) (a > b ? a : b)

typedef struct Vector {
	size_t len;
	size_t cap;
	char buf[0];
} Vector;
#define vec_len(b) ((b) ? vec_hdr(b)->len : 0)
#define vec_cap(b) ((b) ? vec_hdr(b)->cap : 0)
#define vec_hdr(b) ((Vector*) ((char*)b - offsetof(Vector, buf)))
#define vec_set_cap(b, cap) (vec_hdr(b)->cap = cap)
#define vec_fits(b, n) (vec_len(b) + (n) <= vec_cap(b))
#define vec_fit(b, n) (vec_fits(b, n) ? 0 : ((b) = vec_grow((b), vec_len(b) + (n), sizeof(*(b)))))
#define vec_push(b, x) (vec_fit(b, 1), b[vec_len(b)] = (x), vec_hdr(b)->len++)
#define vec_set(b, c, x) (assert(c >= 0), assert(c < vec_len(b)), b[c] = (x) )
#define vec_rem(b, index) (assert(index >=0), assert(index < vec_len(b)), memmove(&b[index], &b[index + 1], (vec_len(b) - 1) * sizeof(b[0])), vec_hdr(b)->len--)
#define vec_free(b) ((b) ? free(vec_hdr(b)) : 0)
#define vec_erase(b, start, end) memmove(&b[start], &b[end * start + 1], (vec_len(b) - 1) * sizeof(b[0]))
void *vec_grow(const void *buf, size_t new_len, size_t elem_size); */



// Inspired by Strechy Buffers by Sean Barett
// This is a minimal implementation of the concept
#define raw_p(buf)		((size_t*)(void*)(buf) - 2) // Allow for two size_t variables: size and cap
#define vec_size(buf)	(*raw_p(buf))
#define vec_cap(buf)	(*(raw_p(buf) + 1))
#define should_grow(buf) (!buf || vec_size(buf) == vec_cap(buf))
#define vec_fit(buf, n) (should_grow(buf) ? buf = __reallocvec__(buf, sizeof(*buf)) : 0)
#define vec_push(buf, elem) ((vec_fit(buf, 1), (buf)[vec_size(buf)++] = elem))
#define vec_rem(buf, index) memmove(&*(buf + index), &*((buf + index + 1)), vec_size(buf) * sizeof(*buf)), vec_size(buf)--
#define vec_erase(buf, start, end) memmove(&*(buf + start), &*((buf + end * start + 1)), vec_size(buf) * sizeof(*buf)), vec_size(buf) -= end - start
void *__reallocvec__(void* buf, size_t buf_size);

// Utils
#define encode(type, value, buf, n) *((type*)(void*)(buf) - n) = value
#define decode(type, buf, n) *((type*)(void*)(buf) - n)
int num_len(unsigned d);
const char *ntos(unsigned n);