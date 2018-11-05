#define BMP_HEADER 122
#define BMP_DATA 5992704
#define BMP_DATA_BEGIN 0x7A
#define TWO_GB_SIZE 2147483648

#define PERMUTE_LOW_MASK 0x72
#define UNPERMUTE_LOW_MASK 0x72
#define PERMUTE_HIGH_MASK 0x72
#define UNPERMUTE_HIGH_MASK 0x8D


#if _MSC_VER && !__INTEL_COMPILER
	#pragma message ("Compiler: MSC")
	#define xAllignedAlloc(align, size) _aligned_malloc(size, align)
	#define xIMG1_PATH "B:\\Korszeru\\korszeru1.bmp"
	#define xIMG2_PATH "B:\\Korszeru\\korszeru2.bmp"
#elif __GNUC__
	#warning "Compiler: GCC"
	#define xAllignedAlloc(align, size) aligned_alloc(align, size)
	#define xIMG1_PATH "/korszeru/korszeru1.bmp"
	#define xIMG2_PATH "/korszeru/korszeru2.bmp"
#else
	#error Not supported compiler
#endif
