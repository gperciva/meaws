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


monowav* monowav_writeOpen(const char filename)
{
	monowav* data = malloc(sizeof(monowav),1);
	file = fopen(filename, "wb");

	written_ = 0;

	hdr_.riff[0] = 'R';
	hdr_.riff[1] = 'I';
	hdr_.riff[2] = 'F';
	hdr_.riff[3] = 'F';

	hdr_.file_size = 44;

	hdr_.wave[0] = 'W';
	hdr_.wave[1] = 'A';
	hdr_.wave[2] = 'V';
	hdr_.wave[3] = 'E';

	hdr_.fmt[0] = 'f';
	hdr_.fmt[1] = 'm';
	hdr_.fmt[2] = 't';
	hdr_.fmt[3] = ' ';

}

