#include <stdio.h>
#include <immintrin.h>
#include "../XDefs.h"
#define LIMIT 5.21

int main()
{
	FILE* f;
	int i;
	float *buffer, *result_buff;
	buffer = (float*)xAllignedAlloc(32, TWO_GB_SIZE);
	result_buff = (float*)xAllignedAlloc(32, 32);


	f = fopen(xFLOATS_PATH, "rb");

	fread(buffer, TWO_GB_SIZE, 1, f);

	__m256 result = _mm256_setzero_ps();
	__m256 mm_limit = _mm256_set1_ps(LIMIT);
	for (i = 0; i < TWO_GB_SIZE / 4; i += 8)
	{
		__m256 mm_a = _mm256_load_ps(buffer + i);
		__m256 mask = _mm256_cmp_ps(mm_a, mm_limit, _CMP_LT_OQ);
		mm_a = _mm256_and_ps(mm_a, mask);
		result = _mm256_add_ps(result, mm_a);
	}
	_mm256_store_ps(result_buff, result);

	float sum = 0;
	for (i = 0; i < 8; i++)
	{
		sum += result_buff[i];
		//printf("%d.: %f \n", i, result_buff[i]);
	}
	printf("%f\n", sum);

	return 0;
}
