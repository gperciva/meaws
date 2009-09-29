#include "monowav.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	char riff[4];           // "RIFF"
	signed int file_size;      // in bytes

	char wave[4];           // "WAVE"

	char fmt[4];            // "fmt "
	signed int chunk_size;     // in bytes (16 for PCM)
	signed short format_tag;     // 1=PCM, 2=ADPCM, 3=IEEE float, 6=A-Law, 7=Mu-Law
	signed short num_chans;      // 1=mono, 2=stereo
	signed int sample_rate;
	signed int bytes_per_sec;
	signed short bytes_per_samp; // 2=16-bit mono, 4=16-bit stereo
	signed short bits_per_samp;
	char data[4];           // "data"
	signed int data_length;    // in bytes
} WavHeader;


int monowav_write(const char *filename, const short *buffer,
                  const unsigned long bufferLength)
{
	WavHeader header;
	FILE *file = fopen(filename, "wb");

	header.riff[0] = 'R';
	header.riff[1] = 'I';
	header.riff[2] = 'F';
	header.riff[3] = 'F';

	header.file_size = 44 + 2 * bufferLength;

	header.wave[0] = 'W';
	header.wave[1] = 'A';
	header.wave[2] = 'V';
	header.wave[3] = 'E';

	header.fmt[0] = 'f';
	header.fmt[1] = 'm';
	header.fmt[2] = 't';
	header.fmt[3] = ' ';

	header.chunk_size = 16;
	header.format_tag = 1;
	header.num_chans = 1;
	header.sample_rate = 44100;
	header.bytes_per_sec = 2 * header.sample_rate;
	header.bytes_per_samp = 2;
	header.bits_per_samp = 16;
	header.data_length = 0;

	header.data[0] = 'd';
	header.data[1] = 'a';
	header.data[2] = 't';
	header.data[3] = 'a';

	fwrite(&header, 4, 11, file);

	fwrite(buffer, sizeof(short), bufferLength, file);

	fclose(file);
	return 0;
}


