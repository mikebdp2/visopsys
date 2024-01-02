//
//  Visopsys
//  Copyright (C) 1998-2023 J. Andrew McLaughlin
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or (at your option)
//  any later version.
//
//  This program is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
//  for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
//  kernelCryptHashMd5.c
//

// This file contains an implementation of the MD5 one-way hashing algorithm,
// useful for passwords and whatnot.
// Ref: RFC 1321  http://www.freesoft.org/CIE/RFC/1321/

#include "kernelCrypt.h"
#include "kernelError.h"
#include <string.h>


static unsigned T[64] = {
	0xD76AA478, /* 1 */   0xE8C7B756, /* 2 */
	0x242070DB, /* 3 */   0xC1BDCEEE, /* 4 */
	0xF57C0FAF, /* 5 */   0x4787C62A, /* 6 */
	0xA8304613, /* 7 */   0xFD469501, /* 8 */
	0x698098D8, /* 9 */   0x8B44F7AF, /* 10 */
	0xFFFF5BB1, /* 11 */  0x895CD7BE, /* 12 */
	0x6B901122, /* 13 */  0xFD987193, /* 14 */
	0xA679438E, /* 15 */  0x49B40821, /* 16 */

	0xF61E2562, /* 17 */  0xC040B340, /* 18 */
	0x265E5A51, /* 19 */  0xE9B6C7AA, /* 20 */
	0xD62F105D, /* 21 */  0x02441453, /* 22 */
	0xD8A1E681, /* 23 */  0xE7D3FBC8, /* 24 */
	0x21E1CDE6, /* 25 */  0xC33707D6, /* 26 */
	0xF4D50D87, /* 27 */  0x455A14ED, /* 28 */
	0xA9E3E905, /* 29 */  0xFCEFA3F8, /* 30 */
	0x676F02D9, /* 31 */  0x8D2A4C8A, /* 32 */

	0xFFFA3942, /* 33 */  0x8771F681, /* 34 */
	0x6D9D6122, /* 35 */  0xFDE5380C, /* 36 */
	0xA4BEEA44, /* 37 */  0x4BDECFA9, /* 38 */
	0xF6BB4B60, /* 39 */  0xBEBFBC70, /* 40 */
	0x289B7EC6, /* 41 */  0xEAA127FA, /* 42 */
	0xD4EF3085, /* 43 */  0x04881D05, /* 44 */
	0xD9D4D039, /* 45 */  0xE6DB99E5, /* 46 */
	0x1FA27CF8, /* 47 */  0xC4AC5665, /* 48 */

	0xF4292244, /* 49 */  0x432AFF97, /* 50 */
	0xAB9423A7, /* 51 */  0xFC93A039, /* 52 */
	0x655B59C3, /* 53 */  0x8F0CCC92, /* 54 */
	0xFFEFF47D, /* 55 */  0x85845DD1, /* 56 */
	0x6FA87E4F, /* 57 */  0xFE2CE6E0, /* 58 */
	0xA3014314, /* 59 */  0x4E0811A1, /* 60 */
	0xF7537E82, /* 61 */  0xBD3AF235, /* 62 */
	0x2AD7D2BB, /* 63 */  0xEB86D391, /* 64 */
};

#define F(X, Y, Z) (((X) & (Y)) | ((~X) & (Z)))
#define G(X, Y, Z) (((X) & (Z)) | ((Y) & (~Z)))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define I(X, Y, Z) ((Y) ^ ((X) | (~Z)))


static inline unsigned rol(unsigned x, unsigned n)
{
	return ((x << n) | (x >> (32 - n)));
}


static void hashChunk(const unsigned *uBuff, unsigned *hash)
{
	// Hash one 512-bit chunk

	unsigned A, B, C, D;
	unsigned AA, BB, CC, DD;
	unsigned bufcnt, count;

	AA = A = hash[0];
	BB = B = hash[1];
	CC = C = hash[2];
	DD = D = hash[3];

	// Don't spend too much time trying to analyze this bit.  It's a somewhat
	// optimized version of the already-convoluted algorithm described in the
	// RFC.  The RFC's description is a lot more readable.

	// Round 1
	for (count = 0; count < 16; count += 4)
	{
		A = B + rol((A + F(B, C, D) + uBuff[count] + T[count]), 7);
		D = A + rol((D + F(A, B, C) + uBuff[count + 1] + T[count + 1]), 12);
		C = D + rol((C + F(D, A, B) + uBuff[count + 2] + T[count + 2]), 17);
		B = C + rol((B + F(C, D, A) + uBuff[count + 3] + T[count + 3]), 22);
	}

	// Round 2
	for (bufcnt = 1; count < 32; bufcnt = ((bufcnt + 20) % 16))
	{
		A = B + rol((A + G(B, C, D) + uBuff[bufcnt] + T[count++]), 5);
		D = A + rol((D + G(A, B, C) + uBuff[(bufcnt + 5) % 16] +
			T[count++]), 9);
		C = D + rol((C + G(D, A, B) + uBuff[(bufcnt + 10) % 16] +
			T[count++]), 14);
		B = C + rol((B + G(C, D, A) + uBuff[(bufcnt + 15) % 16] +
			T[count++]), 20);
	}

	// Round 3
	for (bufcnt = 5; count < 48; bufcnt = ((bufcnt + 12) % 16))
	{
		A = B + rol((A + H(B, C, D) + uBuff[bufcnt] + T[count++]), 4);
		D = A + rol((D + H(A, B, C) + uBuff[(bufcnt + 3) % 16] +
			T[count++]), 11);
		C = D + rol((C + H(D, A, B) + uBuff[(bufcnt + 6) % 16] +
			T[count++]), 16);
		B = C + rol((B + H(C, D, A) + uBuff[(bufcnt + 9) % 16] +
			T[count++]), 23);
	}

	// Round 4
	for (bufcnt = 0; count < 64; bufcnt = ((bufcnt + 28) % 16))
	{
		A = B + rol((A + I(B, C, D) + uBuff[bufcnt] + T[count++]), 6);
		D = A + rol((D + I(A, B, C) + uBuff[(bufcnt + 7) % 16] +
			T[count++]), 10);
		C = D + rol((C + I(D, A, B) + uBuff[(bufcnt + 14) % 16] +
			T[count++]), 15);
		B = C + rol((B + I(C, D, A) + uBuff[(bufcnt + 21) % 16] +
			T[count++]), 21);
	}

	hash[0] = (A + AA);
	hash[1] = (B + BB);
	hash[2] = (C + CC);
	hash[3] = (D + DD);
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
//  Below here, the functions are exported for external use
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

int kernelCryptHashMd5(const unsigned char *message, unsigned len,
	unsigned char *output, int finalize, unsigned totalBytes)
{
	// Hash the message with the constant starting hash.  This is a wrapper
	// around kernelCryptHashMd5Cont(), which will check parameters.

	int status = 0;
	unsigned start[CRYPT_HASH_MD5_BYTES / 4];

	// Fill in starting hash values, as big-endian dwords
	start[0] = 0x67452301;
	start[1] = 0xEFCDAB89;
	start[2] = 0x98BADCFE;
	start[3] = 0x10325476;

	status = kernelCryptHashMd5Cont(message, len, (unsigned char *) start,
		finalize, totalBytes);
	if (status < 0)
		return (status);

	memcpy(output, start, sizeof(start));

	return (status = 0);
}


int kernelCryptHashMd5Cont(const unsigned char *message, unsigned len,
	unsigned char *output, int finalize, unsigned totalBytes)
{
	// Hash the next message with the starting hash given in 'output'

	int status = 0;
	unsigned hash[CRYPT_HASH_MD5_BYTES / 4];
	unsigned chunk[16];
	unsigned lastMessage[32];

	// Check params
	if (!message || !output)
	{
		kernelError(kernel_error, "NULL parameter");
		return (status = ERR_NULLPARAMETER);
	}

	if (!finalize && (len % 64))
	{
		kernelError(kernel_error, "Non-final message must be a multiple of "
			"512 bits");
		return (status = ERR_RANGE);
	}

	memcpy(hash, output, sizeof(hash));

	// Do all the full chunks
	while (len >= 64)
	{
		memcpy(chunk, message, sizeof(chunk));
		hashChunk(chunk, hash);
		message += 64;
		len -= 64;
	}

	if (finalize)
	{
		// Do the last chunk(s)

		memset(lastMessage, 0, sizeof(lastMessage));
		memcpy(lastMessage, message, len);

		// Append a '1' bit
		((unsigned char *) lastMessage)[len] = 0x80;

		// Append 64-bit length value.  Really only 32-bits in a 64-bit field.
		if (len <= 55)
		{
			lastMessage[14] = (totalBytes << 3);
			hashChunk(lastMessage, hash);
		}
		else
		{
			lastMessage[30] = (totalBytes << 3);
			hashChunk(lastMessage, hash);
			hashChunk(&lastMessage[16], hash);
		}

		// Scrub
		memset(lastMessage, 0, len);
	}

	memcpy(output, hash, sizeof(hash));

	return (status = 0);
}

