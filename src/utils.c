#include "utils.h"

uint24_t getLargestFreeHeapFragment() {
	uint24_t try_len = HEAP_LEN;

	// find largest kmalloc contiguous region
	for (; try_len > 0; try_len-=8) {
		void *p = umm_malloc(try_len);
		if (p) {
			umm_free(p);
			break;
		}
	}
	return try_len;
}
