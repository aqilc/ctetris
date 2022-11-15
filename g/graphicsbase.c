#include "../vec.h"


static struct {
	u32 size;
	u32 num;
	struct {
		u16 vertices; // Stores how many vertices this shape has
		u32 index; // Stores index of shape
		u32 cursize; // Stores size at this point
		u64 hash;
	} shps[];
} s;

void scene() {}
void scenediff() {}
