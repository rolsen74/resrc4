#ifndef MD5_H
#define MD5_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "Resourcer/Types.h"

typedef struct
{
	U64		size;			// Size of input in bytes
	U32		buffer[4];		// Current accumulation of hash
	U8		input[64];		// Input to be used in the next step
	U8		digest[16];		// Result of algorithm

} MD5Context;

void md5Init(MD5Context *ctx);
void md5Update(MD5Context *ctx, MEM input, U64 input_len);
void md5Finalize(MD5Context *ctx);
void md5Step(U32 *buffer, U32 *input);

#endif
