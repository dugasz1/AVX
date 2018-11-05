#define BMP_HEADER 122
#define BMP_DATA 5992704
#define BMP_DATA_BEGIN 0x7A
#define TWO_GB_SIZE 2147483648

#if _MSC_VER && !__INTEL_COMPILER
	#warning "Using MSC"
	#define xAllignedAlloc(align, size) _aligned_malloc(size, align)
	#define xIMG1_PATH "B:\\Korszeru\\korszeru1.bmp"
	#define xIMG2_PATH "B:\\Korszeru\\korszeru2.bmp"
#elif __GNUC__
	#warning "Using GCC"
	#define xAllignedAlloc(align, size) aligned_alloc(align, size)
	#define xIMG1_PATH "/korszeru/korszeru1.bmp"
	#define xIMG2_PATH "/korszeru/korszeru2.bmp"
#else
	#error Not supported compiler
#endif
