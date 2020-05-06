#pragma once

#include <stddef.h>

#define LEN(a) ((sizeof(a)) / sizeof(*a))

// WORKS ON ARRAYS, NOT POINTERS.
#define FOREACH(T, v, a) for(size_t i = 0; i < LEN(a); i++) { T p = a[i];

#define END }
