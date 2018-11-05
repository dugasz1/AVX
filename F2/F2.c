#include <stdio.h>
#include <immintrin.h>
#include "../XDefs.h"

int main()
{
	FILE* f;
	int i;
	float *buffer, *result_buff;
	float limit[] = { 5.21, 5.21, 5.21, 5.21, 5.21, 5.21, 5.21, 5.21 };
	buffer = (float*)xAllignedAlloc(32, TWO_GB_SIZE);
	result_buff = (float*)xAllignedAlloc(32, 32);


	f = fopen("/korszeru/floats.2gb.bin", "rb");

	fread(buffer, TWO_GB_SIZE, 1, f);

	__m256 result = _mm256_xor_ps(result, result);
	__m256 b = _mm256_load_ps(limit);
	for (i = 0; i < TWO_GB_SIZE; i += 8)
	{
		__m256 a = _mm256_load_ps(buffer + i);
		__m256 mask = _mm256_cmp_ps(a, b, _CMP_GT_OQ);
		__m256 greather = _mm256_and_ps(a, mask);
		__m256 result = _mm256_and_ps(result, greather);
	}
	_mm256_store_ps(result_buff, result);
	for (i = 0; i < 8; i++)
	{
		printf("%d.: %f \n", i, result_buff[i]);
	}
}
