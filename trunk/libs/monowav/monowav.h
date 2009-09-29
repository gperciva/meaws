/*
** Copyright (C) 1998-2006 George Tzanetakis <gtzan@cs.uvic.ca>
** Copyright (C) 2009 Graham Percival <graham@percival-music.ca>
**   (converted to C and made into a separate library)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
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
} Wavheader;

typedef struct
{
	Wavheader header;
	FILE* file;
	unsigned long written;
	unsigned long beginning;
} Monowav;


Monowav* monowav_writeOpen(const char *filename);
int monowav_writeBuffer(Monowav* data,
	const int *buffer, const int bufferLength);



