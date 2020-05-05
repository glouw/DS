#pragma once

#include <stddef.h>

#define LEN(a) ((sizeof(a)) / sizeof(*a))

#define FOREACH(T, v, a) for(size_t i = 0; i < LEN(a); i++) { T p = a[i];

#define END }
