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


	f = fopen(xFLOATS_PATH, "rb");

	fread(buffer, TWO_GB_SIZE, 1, f);

	__m256 result = _mm256_setzero_ps();
	__m256 b = _mm256_load_ps(limit);
	for (i = 0; i < TWO_GB_SIZE / 4; i += 8)
	{
		__m256 a = _mm256_load_ps(buffer + i);
		_mm256_store_ps(result_buff, a);
		__m256 mask = _mm256_cmp_ps(a, b, _CMP_GT_OQ);
		_mm256_store_ps(result_buff, mask);
		a = _mm256_and_ps(a, mask);
		_mm256_store_ps(result_buff, a);
		result = _mm256_add_ps(result, a);
	}
	_mm256_store_ps(result_buff, result);

	float sum = 0;
	for (i = 0; i < 8; i++)
	{
		sum += result_buff[i];
		//printf("%d.: %f \n", i, result_buff[i]);
	}
	printf("%f", sum);

	return 0;
}
