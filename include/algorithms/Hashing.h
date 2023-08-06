#ifndef ALGORITHMS_HASHING
#define ALGORITHMS_HASHING

#include "data-structures/VoidBuffer.h"
#include <stdint.h>

// The djb2 hashing algorithm. Reference: http://www.cse.yorku.ca/~oz/hash.html
uint64_t Hashing_Djb2(const VoidBuffer* buffer);

#endif 
