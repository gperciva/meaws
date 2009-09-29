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

#include "monowav.h"

monowav* monowav_writeOpen(const char filename)
{
	monowav* data = malloc(sizeof(monowav));
	data.file = fopen(filename, "wb");

	written_ = 0;

	data.header.riff[0] = 'R';
	data.header.riff[1] = 'I';
	data.header.riff[2] = 'F';
	data.header.riff[3] = 'F';

	data.header.file_size = 44;

	data.header.wave[0] = 'W';
	data.header.wave[1] = 'A';
	data.header.wave[2] = 'V';
	data.header.wave[3] = 'E';

	data.header.fmt[0] = 'f';
	data.header.fmt[1] = 'm';
	data.header.fmt[2] = 't';
	data.header.fmt[3] = ' ';

#if defined(MARSYAS_BIGENDIAN)
	data.header.chunk_size = ByteSwapLong(16);
	data.header.format_tag = ByteSwapShort(1);
	data.header.num_chans = ByteSwapShort((signed short)nChannels);
	data.header.sample_rate = ByteSwapLong((mrs_natural)getctrl("mrs_real/israte")->to<mrs_real>());
	data.header.bytes_per_sec = ByteSwapLong(data.header.sample_rate * 2);
	data.header.bytes_per_samp = ByteSwapShort(2);
	data.header.bits_per_samp = ByteSwapShort(16);
	data.header.data_length = ByteSwapLong(0);
#else
	data.header.chunk_size = 16;
	data.header.format_tag = 1;
	data.header.num_chans = (signed short)nChannels;
	data.header.sample_rate = (mrs_natural)getctrl("mrs_real/israte")->to<mrs_real>();
	data.header.bytes_per_sec = data.header.sample_rate * 2;
	data.header.bytes_per_samp = 2;
	data.header.bits_per_samp = 16;
	data.header.data_length = 0;
#endif

	data.header.data[0] = 'd';
	data.header.data[1] = 'a';
	data.header.data[2] = 't';
	data.header.data[3] = 'a';

	fwrite(&data.header, 4, 11, data.file);

	beginning = ftell(sfp_);
}



