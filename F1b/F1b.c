#include <stdio.h>
#include <immintrin.h>
#include "../XDefs.h"

int main()
{
	unsigned char *img1_buff, *img2_buff, *out_buff;
	unsigned char *bmp_header;
	unsigned char *result_buff;

	FILE *img1, *img2, *img_out;
	
	int i;
	short eighty = 205; //round(256 * 0,8)
	short twenty = 51; //round(256 * 0,2)

	//5992826 - 122 = 5992704 | 1632 x 1224 x 3 = 5992704
	img1_buff = (unsigned char *)xAllignedAlloc(32, 5992704); 
	img2_buff = (unsigned char *)xAllignedAlloc(32, 5992704); //5992704 % 32 = 0
	out_buff = (unsigned char *)xAllignedAlloc(32, 5992704);
	bmp_header = (unsigned char *)xAllignedAlloc(32, 122);
	result_buff = (unsigned char *)xAllignedAlloc(32, 32);

	img1 = fopen(xIMG1_PATH, "rb");
	img2 = fopen(xIMG2_PATH, "rb");
	img_out = fopen("f1b.out.bmp", "wb");

	fread(bmp_header, 122, 1, img1); //"seek" to the pos
	fseek(img2, 0x7A, SEEK_SET);

	fread(img1_buff, BMP_DATA, 1, img1);
	fread(img2_buff, BMP_DATA, 1, img2);

	__m256i zeros = _mm256_setzero_si256();
	__m256i mm_eighty = _mm256_set1_epi16(eighty);
	__m256i mm_twenty = _mm256_set1_epi16(twenty);

	for (i = 0; i < 5992704; i += 16)
	{
		//printf("Address: %p | Val: %u \n", img1_buff + i, *(img1_buff+i) );
		__m256i mm_a = _mm256_loadu_si256((__m256i *)(img1_buff + i));
		mm_a = _mm256_permute4x64_epi64(mm_a, 0xD8); //0xD8
		mm_a = _mm256_unpacklo_epi8(mm_a, zeros);
		mm_a = _mm256_mullo_epi16(mm_a, mm_eighty);
		_mm256_store_si256((__m256i *)(result_buff), mm_a);

		__m256i mm_b = _mm256_loadu_si256((__m256i *)(img2_buff + i));
		mm_b = _mm256_permute4x64_epi64(mm_b, 0xD8);
		mm_b = _mm256_unpacklo_epi8(mm_b, zeros);
		mm_b = _mm256_mullo_epi16(mm_b, mm_twenty);
		_mm256_store_si256((__m256i *)(result_buff), mm_b);

		__m256i mm_c = _mm256_add_epi16(mm_a, mm_b);
		_mm256_store_si256((__m256i *)(result_buff), mm_c);
		mm_c = _mm256_srli_epi16(mm_c, 8);
		mm_c = _mm256_packus_epi16(mm_c, zeros);
		mm_c = _mm256_permute4x64_epi64(mm_c, 0xD8);
		_mm256_storeu_si256((__m256i *)(out_buff + i), mm_c);
	}

	fwrite(bmp_header, BMP_HEADER, 1, img_out);
	fwrite(out_buff, BMP_DATA, 1, img_out);

	fflush(img_out);
	fclose(img_out);
}
