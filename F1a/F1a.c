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
	img1_buff = (unsigned char *)xAllignedAlloc(32, 5992704); 
	img2_buff = (unsigned char *)xAllignedAlloc(32, 5992704);
	out_buff = (unsigned char *)xAllignedAlloc(32, 5992704);
	bmp_header = (unsigned char *)xAllignedAlloc(32, 122);

	img1 = fopen(xIMG1_PATH, "rb");
	img2 = fopen(xIMG2_PATH, "rb");
	img_out = fopen("f1a.out.bmp", "wb");

	fread(bmp_header, 122, 1, img1);
	fseek(img2, 0x7A, SEEK_SET);

	fread(img1_buff, 5992704, 1, img1);
	fread(img2_buff, 5992704, 1, img2);

	for (i = 0; i < 5992704; i += 32)
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
