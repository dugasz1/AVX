#include <stdio.h>
#include <immintrin.h>
#include "../XDefs.h"

int main()
{
	unsigned char *img1_buff, *img2_buff, *out_buff_f1a, *out_buff_f1b;
	unsigned char *bmp_header;
	unsigned char *result_buff;

	FILE *img1, *img2, *img_out_f1a, *img_out_f1b;

	int i;
	short eighty = 204; //round(255 * 0,8)
	short twenty = 51; //round(255 * 0,2)

	//5992826 - 122 = 5992704 | 1632 x 1224 x 3 = 5992704
	img1_buff = (unsigned char *)xAllignedAlloc(32, BMP_DATA); //5992704 % 32 = 0
	img2_buff = (unsigned char *)xAllignedAlloc(32, BMP_DATA);
	out_buff_f1a = (unsigned char *)xAllignedAlloc(32, BMP_DATA);
	out_buff_f1b = (unsigned char *)xAllignedAlloc(32, BMP_DATA);
	bmp_header = (unsigned char *)xAllignedAlloc(32, BMP_HEADER);
	result_buff = (unsigned char *)xAllignedAlloc(32, 32);

	img1 = fopen(xIMG1_PATH, "rb");
	img2 = fopen(xIMG2_PATH, "rb");
	img_out_f1a = fopen(F1b_PATH, "wb");
	img_out_f1b = fopen(F1a_PATH, "wb");

	fread(bmp_header, BMP_HEADER, 1, img1); //"seek" to the pos
	fseek(img2, BMP_HEADER, SEEK_SET);

	fread(img1_buff, BMP_DATA, 1, img1);
	fread(img2_buff, BMP_DATA, 1, img2);
	/*fclose(img1);
	fclose(img2);*/

	__m256i zeros = _mm256_setzero_si256();
	__m256i mm_eighty = _mm256_set1_epi16(eighty);
	__m256i mm_twenty = _mm256_set1_epi16(twenty);

	for (i = 0; i < BMP_DATA; i += 32)
	{
		//Img1
		//mm_<feladat1(a|b)>_<variablename>
		__m256i mm_f1b_a_l = _mm256_load_si256((__m256i *)(img1_buff + i));
		__m256i mm_f1a_a = _mm256_load_si256(&mm_f1b_a_l);
		__m256i mm_f1b_a_h;

		mm_f1b_a_l = _mm256_permute4x64_epi64(mm_f1b_a_l, 0xD8);
		mm_f1b_a_h = _mm256_unpackhi_epi8(mm_f1b_a_l, zeros);
		mm_f1b_a_l = _mm256_unpacklo_epi8(mm_f1b_a_l, zeros);

		mm_f1b_a_l = _mm256_mullo_epi16(mm_f1b_a_l, mm_eighty);
		mm_f1b_a_h = _mm256_mullo_epi16(mm_f1b_a_h, mm_eighty);

		//Img2
		__m256i mm_f1b_b_l = _mm256_load_si256((__m256i *)(img2_buff + i));
		__m256i mm_f1a_b = _mm256_load_si256(&mm_f1b_b_l);
		__m256i mm_f1b_b_h;

		mm_f1b_b_l = _mm256_permute4x64_epi64(mm_f1b_b_l, 0xD8);
		mm_f1b_b_h = _mm256_unpackhi_epi8(mm_f1b_b_l, zeros);
		mm_f1b_b_l = _mm256_unpacklo_epi8(mm_f1b_b_l, zeros);

		mm_f1b_b_l = _mm256_mullo_epi16(mm_f1b_b_l, mm_twenty);
		mm_f1b_b_h = _mm256_mullo_epi16(mm_f1b_b_h, mm_twenty);


		//Calculation
		//a Low + b Low
		__m256i mm_f1b_c_l = _mm256_add_epi16(mm_f1b_a_l, mm_f1b_b_l);
		mm_f1b_c_l = _mm256_srli_epi16(mm_f1b_c_l, 8);
		mm_f1b_c_l = _mm256_packus_epi16(mm_f1b_c_l, zeros);
		mm_f1b_c_l = _mm256_permute4x64_epi64(mm_f1b_c_l, 0xD8);
		//a High + b High
		__m256i mm_f1b_c_h = _mm256_add_epi16(mm_f1b_a_h, mm_f1b_b_h);
		mm_f1b_c_h = _mm256_srli_epi16(mm_f1b_c_h, 8);
		mm_f1b_c_h = _mm256_packus_epi16(mm_f1b_c_h, zeros);
		mm_f1b_c_h = _mm256_permute4x64_epi64(mm_f1b_c_h, 0x8D);

		//F1b result
		__m256i mm_f1b_merged = _mm256_or_si256(mm_f1b_c_l, mm_f1b_c_h);
		//F1a result
		__m256i mm_f1a_avg = _mm256_avg_epu8(mm_f1a_a, mm_f1a_b);

		_mm256_store_si256((__m256i *)(out_buff_f1a + i), mm_f1b_merged);
		_mm256_store_si256((__m256i *)(out_buff_f1b + i), mm_f1a_avg);

	}

	fwrite(bmp_header, BMP_HEADER, 1, img_out_f1a);
	fwrite(bmp_header, BMP_HEADER, 1, img_out_f1b);
	fwrite(out_buff_f1a, BMP_DATA, 1, img_out_f1a);
	fwrite(out_buff_f1b, BMP_DATA, 1, img_out_f1b);
	

	/*fflush(img_out_f1a);
	fclose(img_out_f1a);
	fflush(img_out_f1b);
	fclose(img_out_f1b);*/
}
