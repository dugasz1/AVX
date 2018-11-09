#define BMP_HEADER 122
#define BMP_DATA 5992704
#define BMP_DATA_BEGIN 0x7A
#define TWO_GB_SIZE 2147483648

#define F1a_PATH "TMN7C4_Bozo_David_F1a.bmp"
#define F1b_PATH "TMN7C4_Bozo_David_F1b.bmp"

#if _MSC_VER && !__INTEL_COMPILER
	#pragma message ("Compiler: MSC")
	#define xAllignedAlloc(align, size) _aligned_malloc(size, align)
	#define xIMG1_PATH "B:\\Korszeru\\korszeru1.bmp"
	#define xIMG2_PATH "B:\\Korszeru\\korszeru2.bmp"
	#define xFLOATS_PATH "B:\\Korszeru\\floats.2gb.bin"
#elif __GNUC__
	#warning "Compiler: GCC"
	#define xAllignedAlloc(align, size) aligned_alloc(align, size)
	#define xIMG1_PATH "/korszeru/korszeru1.bmp"
	#define xIMG2_PATH "/korszeru/korszeru2.bmp"
	#define xFLOATS_PATH "/korszeru/floats.2gb.bin"
#else
	#error Not supported compiler
#endif
