/*
** Copyright (C) 2008 Graham Percival <gperciva@uvic.ca>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
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

#ifndef RHYTHM_DISPLAY_LINES_H
#define RHYTHM_DISPLAY_LINES_H

#include "abstract-display.h"

class RhythmDisplayLines : public AbstractDisplay
{
	Q_OBJECT
public:
	RhythmDisplayLines(QWidget *parent = 0);
	~RhythmDisplayLines();

protected:
	void paintEvent(QPaintEvent *event);
};

#endif

