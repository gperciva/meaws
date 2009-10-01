#include "monowav.h"
#include <math.h>
#include <limits.h>
#include "stdio.h"
#include "stdlib.h"

#define BUFSIZE 4
int main()
{
	unsigned long i;
	monowav_sound *sound;
	sound = malloc(sizeof(monowav_sound));
	sound->length = BUFSIZE;
	sound->buffer = malloc(2*sound->length);
	for (i=0; i<BUFSIZE; i++)
	{
//		buffer[i] = INT_MAX * sin(i);
		sound->buffer[i] = SHRT_MAX * ((float) i / (float)BUFSIZE);
//		buffer[i] = 0;
	}
	monowav_write("foo.wav", sound);

	printf("*** now reading\n");

	monowav_sound *newsound;
	newsound = monowav_read("foo.wav");
	for (i=0; i<newsound->length; i++)
	{
		printf("%hx\n", newsound->buffer[i]);
	}
	printf("newsize: %i\n", newsound->length);

	return 0;
}
