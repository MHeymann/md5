#include <arpa/inet.h>
#include "md5.h"
#include <string.h>

#define TRUE	1
#define FALSE	0

uint32_t K[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint32_t s[64] = {
	7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
	5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
};

/*** Helper Function Prototypes ******************************************/

int get_md5sum_le(FILE *file, uint32_t sum[]);
int get_md5sum_be(FILE *file, uint32_t sum[]);
void md5_round_le(uint32_t *h, uint32_t *w);
void md5_round_be(uint32_t *h, uint32_t *w);
void append_bit_count(uint64_t bit_count, uint32_t *w);
uint32_t rightrotate(uint32_t x, int n);
uint32_t leftrotate(uint32_t x, int n);
void print32_bits(uint32_t x);
void print64_bits(uint64_t x);
uint32_t endianflip(uint32_t x);
void set_bits(uint32_t *A, uint32_t *B, uint32_t *C, uint32_t *D,
		uint32_t F, uint32_t g, uint32_t *w, uint32_t *s, int i);

/*** Functions ***********************************************************/

int get_md5sum(FILE *file, uint32_t sum[]) 
{
	int endianess = 1;
	int ret_val = 0;
	if(*(char *)&endianess == 1) {
		/* little endian */
		ret_val = get_md5sum_le(file, sum);
	} else {
		/* big endian */
		ret_val = get_md5sum_be(file, sum);
	}
	return ret_val;
}

/*** Helper Functions ****************************************************/

int get_md5sum_le(FILE *file, uint32_t sum[]) 
{
	int ret_val = 0;
	uint32_t w[16];
	uint32_t h[4];
	unsigned char *w_char = (unsigned char *)w;
	int i = 0;
	int readc;
	uint64_t bit_count = 0;

	if (file == NULL) {
		return 1;
	}
	if (sum == NULL) {
		return 2;
	}

	memset(w, 0, 16 * sizeof(uint32_t));

	/* little endian */
	h[0] = (0x67452301);
	h[1] = (0xefcdab89);
	h[2] = (0x98badcfe);
	h[3] = (0x10325476);

	while (TRUE) {
		readc = fread(w, 1, 64, file);
		bit_count += readc * 8;
		if (readc == 64) {
			/* run md5 round */
			md5_round_le(h, w);
		} else if (readc < 56) {
			w_char[readc] = (unsigned char)128;
			for (i = readc + 1; i < 56; i++) {
				w_char[i] = '\0';
			}
			append_bit_count(bit_count, w);

			md5_round_le(h, w);
			
			break;
		} else if (readc < 64) {
			w_char[readc] = (unsigned char)128;
			for (i = readc + 1; i < 64; i++) {
				w_char[i] = '\0';
			}

			md5_round_le(h, w);

			for (i = 0; i < 56; i++) {
				w_char[i] = '\0';
			}
			append_bit_count(bit_count, w);

			md5_round_le(h, w);
			break;

		} else {
			printf("some weird reading io error\n");
		}
	}

	sum[0] = ntohl(h[0]);
	sum[1] = ntohl(h[1]);
	sum[2] = ntohl(h[2]);
	sum[3] = ntohl(h[3]);
	
	return ret_val;
}

int get_md5sum_be(FILE *file, uint32_t sum[]) 
{
	int ret_val = 0;
	uint32_t w[16];
	uint32_t h[4];
	unsigned char *w_char = (unsigned char *)w;
	int i = 0;
	int readc;
	uint64_t bit_count = 0;

	if (file == NULL) {
		return 1;
	}
	if (sum == NULL) {
		return 2;
	}

	memset(w, 0, 16 * sizeof(uint32_t));

	h[0] = (0x01234567);
	h[1] = (0x89abcdef);
	h[2] = (0xfedcba98);
	h[3] = (0x76543210);
	for (i = 0; i < 64; i++) {
		K[i] = endianflip(K[i]);
	}

	/* big endian */
	while (TRUE) {
		readc = fread(w, 1, 64, file);
		bit_count += readc * 8;
		if (readc == 64) {
			/* run md5 round */
			md5_round_be(h, w);
		} else if (readc < 56) {
			w_char[readc] = (unsigned char)128;
			for (i = readc + 1; i < 56; i++) {
				w_char[i] = '\0';
			}
			append_bit_count(bit_count, w);

			md5_round_be(h, w);
			
			break;
		} else if (readc < 64) {
			w_char[readc] = (unsigned char)128;
			for (i = readc + 1; i < 64; i++) {
				w_char[i] = '\0';
			}

			md5_round_be(h, w);

			for (i = 0; i < 56; i++) {
				w_char[i] = '\0';
			}
			append_bit_count(bit_count, w);

			md5_round_be(h, w);
			break;

		} else {
			printf("some weird reading io error\n");
		}
	}

	sum[0] = ntohl(h[0]);
	sum[1] = ntohl(h[1]);
	sum[2] = ntohl(h[2]);
	sum[3] = ntohl(h[3]);
	
	return ret_val;
}


void md5_round_le(uint32_t *h, uint32_t *w) {
	uint32_t A, B, C, D, F, g;
	int i;

	A = h[0];
	B = h[1];
	C = h[2];
	D = h[3];

	for (i = 0; i < 16; i++) {
		F = D ^ (B & (C ^ D));
		g = i;
		set_bits(&A, &B, &C, &D, F, g, w, s, i);
	}
	for (i = 16; i < 32; i++) {
		F = C ^ (D & (B ^ C));
		g = (5 * i + 1) & 15;
		set_bits(&A, &B, &C, &D, F, g, w, s, i);
	}
	for (i = 32; i < 48; i++) {
		F = B ^ C ^ D;
		g = (3 * i + 5) & 15;
		set_bits(&A, &B, &C, &D, F, g, w, s, i);
	}
	for (i = 48; i < 64; i++) {
		F = C ^ (B | (~D));
		g = (7 * i) & 15;
		set_bits(&A, &B, &C, &D, F, g, w, s, i);
	}
	
	h[0] = h[0] + A;
	h[1] = h[1] + B;
	h[2] = h[2] + C;
	h[3] = h[3] + D;
}

void md5_round_be(uint32_t *h, uint32_t *w) {
	uint32_t A, B, C, D, F, g;
	int i;

	/* big endian */
	for (i = 0; i < 16; i++) {
		w[i] = endianflip(w[i]);
	}

	A = h[0];
	B = h[1];
	C = h[2];
	D = h[3];

	for (i = 0; i < 16; i++) {
		F = D ^ (B & (C ^ D));
		g = i;
		set_bits(&A, &B, &C, &D, F, g, w, s, i);
	}
	for (i = 16; i < 32; i++) {
		F = C ^ (D & (B ^ C));
		g = (5 * i + 1) % 16;
		set_bits(&A, &B, &C, &D, F, g, w, s, i);
	}
	for (i = 32; i < 48; i++) {
		F = B ^ C ^ D;
		g = (3 * i + 5) % 16;
		set_bits(&A, &B, &C, &D, F, g, w, s, i);
	}
	for (i = 48; i < 64; i++) {
		F = C ^ (B | (~D));
		g = (7 * i) % 16;
		set_bits(&A, &B, &C, &D, F, g, w, s, i);
	}
	
	h[0] = h[0] + A;
	h[1] = h[1] + B;
	h[2] = h[2] + C;
	h[3] = h[3] + D;
}

void append_bit_count(uint64_t bit_count, uint32_t *w)
{
	int endianess = 1;
	int i;
	unsigned char *size = (unsigned char *)&bit_count;
	unsigned char *char_slot = (unsigned char *)(w + 14);
	uint64_t *size_slot = (uint64_t *)(w + 14);
	
	if(*(char *)&endianess == 1) {
		/* little endian */
		size_slot[0] = bit_count;
	} else {
		/* big endian */
		printf("big endian\n");
		for (i = 0; i < 8; i++) {
			char_slot[i] = size[7 - i];
		}
	}
}


uint32_t rightrotate(uint32_t x, int n)
{
	uint32_t temp;
	temp = x << (32 - n);
	x >>= n;
	return (x | temp);
}


uint32_t leftrotate(uint32_t x, int n)
{
	uint32_t temp;
	temp = x >> (32 - n);
	x <<= n;
	return (x | temp);
}

void print32_bits(uint32_t x)
{
	int i;
	uint32_t one = 1;
	char string[33];
	string[32] = '\0';

	for (i = 0; i < 32; i++) {
		if (x & one) {
			string[31 - i] = '1';
		} else {
			string[31 - i] = '0';
		}
		x >>= 1;
	}
	printf("%s\n", string);
}

void print64_bits(uint64_t x)
{
	int i;
	uint64_t one = 1;
	char string[65];
	string[64] = '\0';

	for (i = 0; i < 64; i++) {
		if (x & one) {
			string[63 - i] = '1';
		} else {
			string[63 - i] = '0';
		}
		x >>= 1;
	}
	printf("%s\n", string);
}

uint32_t endianflip(uint32_t x)
{
	unsigned char *c = (unsigned char *)&x;

	c[0] ^= c[3];
	c[3] ^= c[0];
	c[0] ^= c[3];

	c[1] ^= c[2];
	c[2] ^= c[1];
	c[1] ^= c[2];

	return x;
}

void set_bits(uint32_t *A, uint32_t *B, uint32_t *C, uint32_t *D,
		uint32_t F, uint32_t g, uint32_t *w, uint32_t *s, int i)
{
	uint32_t dTemp;
	dTemp = *D;
	*D = *C;
	*C = *B;
	*B = *B + leftrotate((*A + F + K[i] + w[g]), s[i]);
	*A = dTemp;
}
