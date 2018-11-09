#include <stdio.h>
#include <immintrin.h>
#include "../XDefs.h"

int main()
{
	unsigned char *img1_buff, *img2_buff, *out_buff;
	unsigned char *bmp_header;
	FILE *img1, *img2, *img_out;
	int i;

	//5992826 - 122 = 5992704 | 1632 x 1224 x 3 = 5992704
	img1_buff = (unsigned char *)xAllignedAlloc(32, BMP_DATA);
	img2_buff = (unsigned char *)xAllignedAlloc(32, BMP_DATA);
	out_buff = (unsigned char *)xAllignedAlloc(32, BMP_DATA);
	bmp_header = (unsigned char *)xAllignedAlloc(32, BMP_HEADER);

	img1 = fopen(xIMG1_PATH, "rb");
	img2 = fopen(xIMG2_PATH, "rb");
	img_out = fopen(F1a_PATH, "wb");

	fread(bmp_header, BMP_HEADER, 1, img1);
	fseek(img2, BMP_HEADER, SEEK_SET);

	fread(img1_buff, BMP_DATA, 1, img1);
	fread(img2_buff, BMP_DATA, 1, img2);

	for (i = 0; i < BMP_DATA; i += 32)
	{
		__m256i mm_a = _mm256_load_si256((__m256i *)(img1_buff + i));
		__m256i mm_b = _mm256_load_si256((__m256i *)(img2_buff + i));
		__m256i mm_c = _mm256_avg_epu8(mm_a, mm_b);
		_mm256_store_si256((__m256i *)(out_buff + i), mm_c);
	}

	fwrite(bmp_header, BMP_HEADER, 1, img_out);
	fwrite(out_buff, BMP_DATA, 1, img_out);

	fflush(img_out);
	fclose(img_out);
}
