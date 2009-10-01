#include "monowav.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	char riff[4];
	signed int file_size;
	char wave[4];
	char fmt[4];
	signed int chunk_size;
	signed short format_tag;
	signed short num_chans;
	signed int sample_rate;
	signed int bytes_per_sec;
	signed short bytes_per_sample;
	signed short bits_per_sample;
	char data[4];
	signed int data_length;
} WavHeader;


int monowav_write(const char *filename, monowav_sound *sound)
{
	WavHeader header;
	FILE *file = fopen(filename, "wb");

	header.riff[0] = 'R';
	header.riff[1] = 'I';
	header.riff[2] = 'F';
	header.riff[3] = 'F';

	header.file_size = 2 * sound->length - 8;

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
	header.bytes_per_sample = 2;
	header.bits_per_sample = 16;

	header.data[0] = 'd';
	header.data[1] = 'a';
	header.data[2] = 't';
	header.data[3] = 'a';

	header.data_length = 2 * sound->length;

	fwrite(&header, 4, 11, file);

	fwrite(sound->buffer, 2, sound->length, file);

	fclose(file);
	return 0;
}

monowav_sound* monowav_read(const char *filename)
{
	monowav_sound* sound;
        sound = malloc(sizeof(monowav_sound));
	FILE *file = fopen(filename, "rb");

	signed int file_size;
	fseek(file, 40, SEEK_SET);
	fread(&file_size, 4, 1, file);

	sound->length = file_size / 2;
	sound->buffer = malloc( 2 * sound->length);
	fread(sound->buffer, 2, sound->length, file);

	fclose(file);
	sound->status = MONOWAV_OK;
	return sound;
}


