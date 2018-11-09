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
	short eighty = 204; //round(255 * 0,8)
	short twenty = 51; //round(255 * 0,2)

	//5992826 - 122 = 5992704 | 1632 x 1224 x 3 = 5992704
	img1_buff = (unsigned char *)xAllignedAlloc(32, BMP_DATA); //5992704 % 32 = 0
	img2_buff = (unsigned char *)xAllignedAlloc(32, BMP_DATA); 
	out_buff = (unsigned char *)xAllignedAlloc(32, BMP_DATA);
	bmp_header = (unsigned char *)xAllignedAlloc(32, BMP_HEADER);
	result_buff = (unsigned char *)xAllignedAlloc(32, 32);

	img1 = fopen(xIMG1_PATH, "rb");
	img2 = fopen(xIMG2_PATH, "rb");
	img_out = fopen("f1b.out.bmp", "wb");

	fread(bmp_header, BMP_HEADER, 1, img1); //"seek" to the pos
	fseek(img2, BMP_HEADER, SEEK_SET);

	fread(img1_buff, BMP_DATA, 1, img1);
	fread(img2_buff, BMP_DATA, 1, img2);

	__m256i zeros = _mm256_setzero_si256();
	__m256i mm_eighty = _mm256_set1_epi16(eighty);
	__m256i mm_twenty = _mm256_set1_epi16(twenty);

	for (i = 0; i < BMP_DATA; i += 32)
	{
		//Img1
		__m256i mm_a_l = _mm256_load_si256((__m256i *)(img1_buff + i));
		__m256i mm_a_h;

		mm_a_l = _mm256_permute4x64_epi64(mm_a_l, 0xD8);
		mm_a_h = _mm256_unpackhi_epi8(mm_a_l, zeros);
		mm_a_l = _mm256_unpacklo_epi8(mm_a_l, zeros);

		mm_a_l = _mm256_mullo_epi16(mm_a_l, mm_eighty);
		mm_a_h = _mm256_mullo_epi16(mm_a_h, mm_eighty);

		//Img2
		__m256i mm_b_l = _mm256_load_si256((__m256i *)(img2_buff + i));
		__m256i mm_b_h;

		mm_b_l = _mm256_permute4x64_epi64(mm_b_l, 0xD8);
		mm_b_h = _mm256_unpackhi_epi8(mm_b_l, zeros);
		mm_b_l = _mm256_unpacklo_epi8(mm_b_l, zeros);

		mm_b_l = _mm256_mullo_epi16(mm_b_l, mm_twenty);
		mm_b_h = _mm256_mullo_epi16(mm_b_h, mm_twenty);


		//Calculation
		//a Low + b Low
		__m256i mm_c_l = _mm256_add_epi16(mm_a_l, mm_b_l);
		mm_c_l = _mm256_srli_epi16(mm_c_l, 8);
		mm_c_l = _mm256_packus_epi16(mm_c_l, zeros);
		mm_c_l = _mm256_permute4x64_epi64(mm_c_l, 0xD8);
		//a High + b High
		__m256i mm_c_h = _mm256_add_epi16(mm_a_h, mm_b_h);
		mm_c_h = _mm256_srli_epi16(mm_c_h, 8);
		mm_c_h = _mm256_packus_epi16(mm_c_h, zeros);
		mm_c_h = _mm256_permute4x64_epi64(mm_c_h, 0x8D);

		__m256i mm_merged = _mm256_or_si256(mm_c_l, mm_c_h);

		_mm256_store_si256((__m256i *)(out_buff + i), mm_merged);
	}

	fwrite(bmp_header, BMP_HEADER, 1, img_out);
	fwrite(out_buff, BMP_DATA, 1, img_out);

	fflush(img_out);
	fclose(img_out);
}
