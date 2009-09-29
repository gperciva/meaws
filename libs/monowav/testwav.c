#include "monowav.h"
#include <math.h>
#include <limits.h>

int main()
{
	short buffer[2048];
	unsigned long i;
	for (i=0; i<2048; i++)
	{
//		buffer[i] = INT_MAX * sin(i);
		buffer[i] = SHRT_MAX * ((float) i / 2048.0);
	}
	monowav_write("foo.wav", &buffer, 2048);

	return 0;
}
