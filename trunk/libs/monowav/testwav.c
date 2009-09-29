#include "monowav.h"
#include <math.h>
#include <limits.h>

#define BUFSIZE 100
int main()
{
	short buffer[BUFSIZE];
	unsigned long i;
	for (i=0; i<BUFSIZE; i++)
	{
//		buffer[i] = INT_MAX * sin(i);
		buffer[i] = SHRT_MAX * ((float) i / (float)BUFSIZE);
//		buffer[i] = 0;
	}
	monowav_write("foo.wav", &buffer, BUFSIZE);

	return 0;
}
