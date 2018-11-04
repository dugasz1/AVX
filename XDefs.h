#define BMP_HEADER 122
#define BMP_DATA 5992704
#define BMP_DATA_BEGIN 0x7A

#ifdef _MSC_VER && !__INTEL_COMPILER
	#define xAllignedAlloc(align, size) _aligned_malloc(size, align)
	#define xIMG1_PATH "B:\\Korszeru\\korszeru1.bmp"
	#define xIMG2_PATH "B:\\Korszeru\\korszeru2.bmp"
#elif __GNUC__
	#define xAllignedAlloc(align, size) aligned_alloc(align, size)
	#define xIMG1_PATH "korszeru/korszeru1.bmp"
	#define xIMG2_PATH "korszeru/korszeru2.bmp"
#else
	#error Not supported compiler
#endif