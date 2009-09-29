#include "monowav.h"
#include <math.h>
#include <limits.h>

int main()
{
	Monowav* foo = monowav_writeOpen("foo.wav");

	int buffer[2048];
	int i;
	for (i=0; i<2048; i++)
	{
		buffer[i] = INT_MAX * sin(i);
	}
//	monowav_writeBuffer(&foo, &buffer, 2048);

	return 0;
}
